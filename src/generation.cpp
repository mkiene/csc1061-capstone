#include "../include/generation.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <future>
#include <iostream>
#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

#include "../include/config.h"
#include "../include/utils.h"

std::vector<std::shared_ptr<Generator>> generators;

bool rooms_overlap(std::shared_ptr<Room> room_a, std::shared_ptr<Room> room_b, int padding)
{
    int x1 = room_a->pos.x;
    int y1 = room_a->pos.y;
    int width1 = room_a->size.x + padding;
    int height1 = room_a->size.y + padding;

    int x2 = room_b->pos.x;
    int y2 = room_b->pos.y;
    int width2 = room_b->size.x + padding;
    int height2 = room_b->size.y + padding;

    return !(x1 + width1 <= x2 ||   // Room 1 is to the left of Room 2
             x2 + width2 <= x1 ||   // Room 2 is to the left of Room 1
             y1 + height1 <= y2 ||  // Room 1 is above Room 2
             y2 + height2 <= y1);   // Room 2 is above Room 1
}

void add_room(std::unordered_map<int, std::vector<std::pair<int, bool>>>& adj_list, int room_id)
{
    if (adj_list.find(room_id) == adj_list.end())
    {
        adj_list[room_id] = {};
    }
}

void generate_MST(const std::vector<std::shared_ptr<Room>>& rooms, std::unordered_map<int, std::vector<std::pair<int, bool>>>& adjacency_list)
{
    int num_rooms = rooms.size();
    std::vector<bool> in_tree(num_rooms, false);    // Track vertices in the MST
    std::priority_queue<std::tuple<int, int, int>,  // (distance, roomA, roomB)
                        std::vector<std::tuple<int, int, int>>,
                        std::greater<>>
        pq;  // Min-heap based on distance

    // Start with the first room
    int start = 0;
    in_tree[start] = true;

    // Add all edges from the starting room to the priority queue
    for (int i = 1; i < num_rooms; i++)
    {
        pq.push({sqr_dist(rooms[start]->pos.x, rooms[start]->pos.y, rooms[i]->pos.x, rooms[i]->pos.y), start, i});
    }

    // Grow the MST
    while (!pq.empty())
    {
        auto [dist, room_a, room_b] = pq.top();
        pq.pop();

        // If roomB is already in the tree, skip
        if (in_tree[room_b])
            continue;

        // Add the edge to the adjacency list with "hallway not yet generated"

        if (adjacency_list.find(room_a) == adjacency_list.end())
            add_room(adjacency_list, room_a);

        if (adjacency_list.find(room_b) == adjacency_list.end())
            add_room(adjacency_list, room_b);

        adjacency_list[room_a].emplace_back(room_b, false);
        adjacency_list[room_b].emplace_back(room_a, false);

        // Mark room_b as part of the tree
        in_tree[room_b] = true;

        // Add all edges from roomB to non-tree vertices
        for (int i = 0; i < num_rooms; i++)
        {
            if (!in_tree[i])
            {
                pq.push({sqr_dist(rooms[room_b]->pos.x, rooms[room_b]->pos.y, rooms[i]->pos.x, rooms[i]->pos.y), room_b, i});
            }
        }
    }
}

void generate_hallway(std::shared_ptr<Room> room_a, std::shared_ptr<Room> room_b, std::shared_ptr<Level> level)
{
    vec2 a = room_a->pos + room_a->size / 2;
    vec2 b = room_b->pos + room_b->size / 2;

    vec2 rel = b - a;

    // Generate horizontal segment
    for (int x = 0; x <= abs(rel.x); x++)
    {
        level->set_tile(a.x + x * rel.sign().x, a.y, {0, '.'});
        level->set_tile(a.x + x * rel.sign().x, a.y - 1, {0, '.'});
        level->set_tile(a.x + x * rel.sign().x, a.y + 1, {0, '.'});
    }

    // Generate vertical segment
    for (int y = 0; y <= abs(rel.y); y++)
    {
        level->set_tile(b.x, a.y + y * rel.sign().y, {0, '.'});
        level->set_tile(b.x - 1, a.y + y * rel.sign().y, {0, '.'});
        level->set_tile(b.x + 1, a.y + y * rel.sign().y, {0, '.'});
    }
}

void Generator::rogue(int room_num, vec2 min_room_size, vec2 max_room_size, int room_padding)
{
    std::vector<std::shared_ptr<Room>> generated_rooms;
    std::unordered_map<int, std::vector<std::pair<int, bool>>> adjacency_list;

    int max_tries = 1000;
    int tries = 0;

    for (int i = 0; i < room_num; i++)
    {
    restart_outer:

        if (tries > max_tries)
            break;

        tries++;

        vec2 new_room_size = {(float)get_random_int(min_room_size.x, max_room_size.x), (float)get_random_int(min_room_size.y, max_room_size.y)};
        vec2 new_room_pos = {(float)get_random_int(0.0f, level->get_size().x - new_room_size.x), (float)get_random_int(0.0f, level->get_size().y - new_room_size.y)};

        auto new_room = std::make_shared<Room>(new_room_pos, new_room_size, i);

        for (int j = 0; j < generated_rooms.size(); j++)
        {
            if (rooms_overlap(new_room, generated_rooms[j], room_padding))
            {
                goto restart_outer;
            }
        }

        generated_rooms.push_back(new_room);
        add_room(adjacency_list, new_room->id);

        for (int y = 0; y < new_room->size.y; y++)
        {
            for (int x = 0; x < new_room->size.x; x++)
            {
                level->set_tile(new_room_pos.x + x, new_room_pos.y + y, {level->get_depth(), '.'});
            }
        }
    }

    entities[0]->set_pos(generated_rooms[0]->pos + generated_rooms[0]->size / 2);

    generate_MST(generated_rooms, adjacency_list);

    for (auto& [room_a, neighbors] : adjacency_list)
    {
        for (auto& [room_b, hallway_generated] : neighbors)
        {
            if (!hallway_generated)
            {
                hallway_generated = true;
                for (auto& [back_room, back_hallway_generated] : adjacency_list[room_b])
                {
                    if (back_room == room_a)
                    {
                        back_hallway_generated = true;
                        break;
                    }
                }
                generate_hallway(generated_rooms[room_a], generated_rooms[room_b], level);
            }
        }
    }
}

void Generator::automata(int iterations, int chance, int birth_threshold, int survival_threshold)
{
    if (level == nullptr)
        return;

    iterations = std::clamp(iterations, 0, 20);
    chance = std::clamp(chance, 0, 100);
    birth_threshold = std::clamp(birth_threshold, 0, 8);
    survival_threshold = std::clamp(survival_threshold, 0, 8);

    std::vector<std::vector<int>> states(level->get_size().y, std::vector<int>(level->get_size().x, 0));
    std::vector<std::vector<int>> future_states(level->get_size().y, std::vector<int>(level->get_size().x, 0));

    // initialize

    for (int i = 0; i < level->get_size().y; i++)
    {
        for (int j = 0; j < level->get_size().x; j++)
        {
            if (get_random_int(0, 100) <= chance || level->get_tile(j, i).symbol == floor_symb)
            {
                states[i][j] = 1;
                future_states[i][j] = 1;
            }
        }
    }

    // apply cellular automata

    for (int n = 0; n < iterations; n++)
    {
        for (int i = 0; i < level->get_size().y; i++)
        {
            for (int j = 0; j < level->get_size().x; j++)
            {
                if (i > 0 && i < level->get_size().y - 1 && j > 0 && j < level->get_size().x - 1)
                {
                    // get neigbor count

                    int alive_neighbors = 0;

                    for (int y = -1; y <= 1; y++)
                    {
                        for (int x = -1; x <= 1; x++)
                        {
                            if (x == 0 && y == 0)
                                continue;
                            if (states[i + y][j + x] == 1)
                            {
                                alive_neighbors++;
                            }
                        }
                    }

                    if (states[i][j] == 0 && alive_neighbors >= birth_threshold)
                    {
                        future_states[i][j] = 1;
                    }
                    else if (states[i][j] == 1 && alive_neighbors >= survival_threshold)
                    {
                        future_states[i][j] = 1;
                    }
                    else
                    {
                        future_states[i][j] = 0;
                    }

                    /* Conway's Game of Life

                    if (states[i][j] == 1)
                    {
                        // Rule for live cells
                        if (alive_neighbors < 2 || alive_neighbors > 3)
                        {
                            future_states[i][j] = 0;  // Dies from underpopulation or overpopulation
                        }
                        else
                        {
                            future_states[i][j] = 1;  // Survives
                        }
                    }
                    else
                    {
                        // Rule for dead cells
                        if (alive_neighbors == 3)
                        {
                            future_states[i][j] = 1;  // Becomes alive due to reproduction
                        }
                        else
                        {
                            future_states[i][j] = 0;  // Remains dead
                        }
                    }

                    */
                }
                else
                {
                    future_states[i][j] = 0;
                }
            }
        }

        std::swap(states, future_states);
    }

    for (int i = 0; i < level->get_size().y; i++)
    {
        for (int j = 0; j < level->get_size().x; j++)
        {
            if (states[i][j] == 1)
            {
                level->set_tile(j, i, {5, floor_symb});
            }
            else
            {
                level->set_tile(j, i, {level->get_tile(j, i).depth, empty_symb});
            }
        }
    }
}

void Generator::remove_inaccessible()
{
    if (level == nullptr)
        return;

    int start_x = entities[0]->get_pos().x;
    int start_y = entities[0]->get_pos().y;

    std::vector<std::vector<bool>> visited(level->get_size().y, std::vector<bool>(level->get_size().x, false));
    std::queue<std::pair<int, int>> q;

    q.push({start_y, start_x});
    visited[start_y][start_x] = true;

    while (!q.empty())
    {
        auto [y, x] = q.front();
        q.pop();

        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};

        for (int dir = 0; dir < 4; dir++)
        {
            int new_x = x + dx[dir];
            int new_y = y + dy[dir];

            if (new_x >= 0 && new_x < level->get_size().x && new_y >= 0 && new_y < level->get_size().y)
            {
                if (!visited[new_y][new_x] && level->get_tile(new_x, new_y).symbol != empty_symb)
                {
                    visited[new_y][new_x] = true;
                    q.push({new_y, new_x});
                }
            }
        }
    }

    for (int i = 0; i < level->get_size().y; i++)
    {
        for (int j = 0; j < level->get_size().x; j++)
        {
            if (!visited[i][j])
            {
                level->set_tile(j, i, {level->get_tile(j, i).depth, empty_symb});
            }
        }
    }
}
