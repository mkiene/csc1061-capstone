/*
 * Max Kiene
 * Sunday, December 8th, 2024
 * CSC1061 Capstone
 */

#include "../include/generation.h"

#include <cmath>
#include <queue>
#include <unordered_map>
#include <vector>

#include "../include/config.h"
#include "../include/entity.h"
#include "../include/room.h"
#include "../include/utils.h"
#include "../include/viewport.h"

// Global vector to store level generators
std::vector<std::shared_ptr<Generator>> generators;

// Check if two rooms overlap, considering padding. This is used to prevent rooms from being placed too close to each other.
bool rooms_overlap(std::shared_ptr<Room> room_a, std::shared_ptr<Room> room_b, int padding)
{
    int x1 = static_cast<int>(room_a->pos.x);
    int y1 = static_cast<int>(room_a->pos.y);
    int width1 = static_cast<int>(room_a->size.x) + padding;
    int height1 = static_cast<int>(room_a->size.y) + padding;

    int x2 = static_cast<int>(room_b->pos.x);
    int y2 = static_cast<int>(room_b->pos.y);
    int width2 = static_cast<int>(room_b->size.x) + padding;
    int height2 = static_cast<int>(room_b->size.y) + padding;

    // Check if bounding boxes of rooms overlap
    return !(x1 + width1 <= x2 || x2 + width2 <= x1 || y1 + height1 <= y2 || y2 + height2 <= y1);
}

// Add a room to the adjacency list if it does not already exist
void add_room(std::unordered_map<int, std::vector<std::pair<int, bool>>>& adj_list, int room_id)
{
    if (adj_list.find(room_id) == adj_list.end())
    {
        adj_list[room_id] = {};
    }
}

/*
 * Generate a Minimum Spanning Tree (MST) to connect all rooms:
 * 1. Start with the first room in the tree.
 * 2. Add the shortest connection (edge) to a new room not yet in the tree.
 * 3. Repeat until all rooms are connected, ensuring no cycles.
 * 4. Store connections in an adjacency list for later use (e.g., hallway generation).
 */

void generate_MST(const std::vector<std::shared_ptr<Room>>& rooms, std::unordered_map<int, std::vector<std::pair<int, bool>>>& adjacency_list)
{
    size_t num_rooms = rooms.size();
    std::vector<bool> in_tree(num_rooms, false);
    std::priority_queue<std::tuple<size_t, size_t, size_t>, std::vector<std::tuple<size_t, size_t, size_t>>, std::greater<>> pq;  // Min-heap to store edges as (distance, room_a, room_b)

    // Start from the first room
    size_t start = 0;
    in_tree[start] = 1;

    // Add edges connecting the starting room to all others
    for (size_t i = 1; i < num_rooms; i++)
    {
        pq.push({sqr_dist(rooms[start]->pos.x, rooms[start]->pos.y, rooms[i]->pos.x, rooms[i]->pos.y), start, i});
    }

    // Process the priority queue to construct the MST
    while (!pq.empty())
    {
        auto [dist, room_a, room_b] = pq.top();
        pq.pop();

        // Skip if the room is already in the MST
        if (in_tree[room_b])
            continue;

        // Ensure rooms exist in the adjacency list
        add_room(adjacency_list, static_cast<int>(room_a));
        add_room(adjacency_list, static_cast<int>(room_b));

        // Add a bidirectional connection between rooms
        adjacency_list[static_cast<int>(room_a)].emplace_back(room_b, false);
        adjacency_list[static_cast<int>(room_b)].emplace_back(room_a, false);

        in_tree[room_b] = true;

        // Add edges from the newly added room to all remaining rooms
        for (size_t i = 0; i < num_rooms; i++)
        {
            if (!in_tree[i])
            {
                pq.push({sqr_dist(rooms[static_cast<size_t>(room_b)]->pos.x, rooms[room_b]->pos.y, rooms[i]->pos.x, rooms[i]->pos.y), room_b, i});
            }
        }
    }
}

// Generate a hallway connecting two rooms by creating a horizontal and vertical passage
void generate_hallway(std::shared_ptr<Room> room_a, std::shared_ptr<Room> room_b, std::shared_ptr<Level> level)
{
    vec2 a = room_a->pos + room_a->size / 2;  // Center of the first room
    vec2 b = room_b->pos + room_b->size / 2;  // Center of the second room

    vec2 rel = b - a;  // Relative position between the two room centers

    // Generate horizontal segment
    for (int x = 0; x <= static_cast<int>(abs(rel.x)); x++)
    {
        level->set_tile(static_cast<int>(a.x) + x * static_cast<int>(rel.sign().x), static_cast<int>(a.y), {0, '.'});
        level->set_tile(static_cast<int>(a.x) + x * static_cast<int>(rel.sign().x), static_cast<int>(a.y) - 1, {0, '.'});
        level->set_tile(static_cast<int>(a.x) + x * static_cast<int>(rel.sign().x), static_cast<int>(a.y) + 1, {0, '.'});
    }

    // Generate vertical segment
    for (int y = 0; y <= static_cast<int>(abs(rel.y)); y++)
    {
        level->set_tile(static_cast<int>(b.x), static_cast<int>(a.y) + y * static_cast<int>(rel.sign().y), {0, '.'});
        level->set_tile(static_cast<int>(b.x) - 1, static_cast<int>(a.y) + y * static_cast<int>(rel.sign().y), {0, '.'});
        level->set_tile(static_cast<int>(b.x) + 1, static_cast<int>(a.y) + y * static_cast<int>(rel.sign().y), {0, '.'});
    }
}

/*
 * Creates a procedurally generated dungeon layout similar to rogue.
 * This includes room placement (no overlap) and optimally connecting rooms with hallways using the MST algorithm.
 *
 * 1. Until the target number of rooms is reached (or an attempt limit), try to place a room.
 * 2. If the room has no overlap, place the room in the level.
 * 3. Connect all rooms together using the MST, which ensures optimal hallways.
 */

void Generator::rogue(int room_num, vec2 min_room_size, vec2 max_room_size, int room_padding)
{
    std::vector<std::shared_ptr<Room>> generated_rooms;                         // Store generated rooms
    std::unordered_map<int, std::vector<std::pair<int, bool>>> adjacency_list;  // Adjacency list for room connections

    int max_tries = 1000;  // Maximum attempts to place rooms
    int tries = 0;

    // Generate the specified number of rooms
    for (int i = 0; i < room_num; i++)
    {
    restart_outer:  // Label for restarting room generation if overlap is detected

        if (tries > max_tries)
            break;

        tries++;

        // Generate a random room size and position
        vec2 new_room_size = vec2(get_random_int(min_room_size.x, max_room_size.x), get_random_int(min_room_size.y, max_room_size.y));
        vec2 new_room_pos = vec2(get_random_int(0.0f, level->get_size().x - new_room_size.x), get_random_int(0.0f, level->get_size().y - new_room_size.y));

        auto new_room = std::make_shared<Room>(new_room_pos, new_room_size, i);

        // Ensure the new room does not overlap with existing rooms
        for (size_t j = 0; j < generated_rooms.size(); j++)
        {
            if (rooms_overlap(new_room, generated_rooms[j], room_padding))
            {
                goto restart_outer;  // Retry room generation
            }
        }

        // Add the new room to the list of generated rooms
        generated_rooms.push_back(new_room);
        add_room(adjacency_list, new_room->id);

        // Fill the level grid with the room's floor tiles
        for (int y = 0; y < static_cast<int>(new_room->size.y); y++)
        {
            for (int x = 0; x < static_cast<int>(new_room->size.x); x++)
            {
                level->set_tile(static_cast<int>(new_room_pos.x) + x, static_cast<int>(new_room_pos.y) + y, {level->get_depth(), '.'});
            }
        }
    }

    // Place the player entity in the first room
    entities[0]->set_pos(generated_rooms[0]->pos + generated_rooms[0]->size / 2);
    viewports[0]->set_level_pos(entities[0]->get_pos() - viewports[0]->get_size() / 2);

    // Generate the MST
    generate_MST(generated_rooms, adjacency_list);

    // Generate hallways based on the MST
    for (auto& [room_a, neighbors] : adjacency_list)
    {
        for (auto& [room_b, hallway_generated] : neighbors)
        {
            if (!hallway_generated)
            {
                hallway_generated = true;  // Mark this hallway as generated
                for (auto& [back_room, back_hallway_generated] : adjacency_list[room_b])
                {
                    if (back_room == room_a)
                    {
                        back_hallway_generated = true;
                        break;
                    }
                }
                generate_hallway(generated_rooms[static_cast<size_t>(room_a)], generated_rooms[static_cast<size_t>(room_b)], level);
            }
        }
    }
}

/*
 * Generate terrain using cellular automata rules.
 * When paired with the roguelike room generation, this leads to playable yet natural-looking levels.
 *
 * 1. Fill the grid with random values of "alive" or "dead" cells.
 * 2. For n iterations, apply the following rules to each cell:
 *      - If the cell is dead and has more than birth_threshold neighbors, the cell becomes alive.
 *      - If the cell is alive and has more than survival_threshold neighbors, the cell survives.
 *      - Otherwise, the cell dies.
 */

void Generator::automata(int iterations, int chance, int birth_threshold, int survival_threshold)
{
    if (level == nullptr)
        return;

    // Clamp values to reasonable ranges to prevent extreme results
    iterations = std::clamp(iterations, 0, 20);
    chance = std::clamp(chance, 0, 100);
    birth_threshold = std::clamp(birth_threshold, 0, 8);
    survival_threshold = std::clamp(survival_threshold, 0, 8);

    // Initialize state grids for the cellular automata
    std::vector<std::vector<int>> states(static_cast<size_t>(level->get_size().y), std::vector<int>(static_cast<size_t>(level->get_size().x), 0));
    std::vector<std::vector<int>> future_states(static_cast<size_t>(level->get_size().y), std::vector<int>(static_cast<size_t>(level->get_size().x), 0));

    // Randomly seed the grid based on the chance parameter
    for (size_t i = 0; i < static_cast<size_t>(level->get_size().y); i++)
    {
        for (size_t j = 0; j < static_cast<size_t>(level->get_size().x); j++)
        {
            if (get_random_int(0, 100) <= chance || level->get_tile(j, i).symbol == floor_symb)  // Includes previously-placed rooms
            {
                states[i][j] = 1;
                future_states[i][j] = 1;
            }
        }
    }

    // Apply the cellular automata rules for the specified number of iterations
    for (int n = 0; n < iterations; n++)
    {
        for (size_t i = 0; i < static_cast<size_t>(level->get_size().y); i++)
        {
            for (size_t j = 0; j < static_cast<size_t>(level->get_size().x); j++)
            {
                if (i > 0 && i < static_cast<size_t>(level->get_size().y) - 1 && j > 0 && j < static_cast<size_t>(level->get_size().x) - 1)
                {
                    // Count the number of alive neighbors for this cell
                    int alive_neighbors = 0;

                    for (int y = -1; y <= 1; y++)
                    {
                        for (int x = -1; x <= 1; x++)
                        {
                            if (x == 0 && y == 0)
                                continue;  // Skip the center cell

                            if (states[static_cast<size_t>(static_cast<int>(i) + y)][static_cast<size_t>(static_cast<int>(j) + x)] == 1)
                            {
                                alive_neighbors++;
                            }
                        }
                    }

                    // Apply birth and survival rules
                    if (states[i][j] == 0 && alive_neighbors >= birth_threshold)
                    {
                        future_states[i][j] = 1;  // Cell becomes alive
                    }
                    else if (states[i][j] == 1 && alive_neighbors >= survival_threshold)
                    {
                        future_states[i][j] = 1;  // Cell remains alive
                    }
                    else
                    {
                        future_states[i][j] = 0;  // Cell dies
                    }
                }
                else
                {
                    future_states[i][j] = 0;  // Border cells are set to dead
                }
            }
        }

        // Update the grid state for the next iteration
        std::swap(states, future_states);
    }

    // Update the level's tiles based on the final grid state
    for (size_t i = 0; i < static_cast<size_t>(level->get_size().y); i++)
    {
        for (size_t j = 0; j < static_cast<size_t>(level->get_size().x); j++)
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

/*
 * Perform a flood fill algorithm to mark accessible tiles starting from a specific position.
 * This ensures that only reachable areas of the dungeon remain.
 * Uses recursion (Requirement)
 */
void flood_fill(size_t x, size_t y, std::vector<std::vector<bool>>& visited, std::shared_ptr<Level> level)
{
    // Base case: Check for boundaries and already visited or empty tiles
    if (x < 0 || x >= static_cast<size_t>(level->get_size().x) || y < 0 || y >= static_cast<size_t>(level->get_size().y))
        return;
    if (visited[y][x] || level->get_tile(x, y).symbol == empty_symb)
        return;

    visited[y][x] = true;

    // Recursively visit all adjacent tiles
    flood_fill(x + 1, y, visited, level);
    flood_fill(x - 1, y, visited, level);
    flood_fill(x, y + 1, visited, level);
    flood_fill(x, y - 1, visited, level);
}

/*
 * Remove all inaccessible tiles from the dungeon via flood fill from the player’s starting position.
 * Inaccessible areas are set to empty tiles.
 */

void Generator::remove_inaccessible()
{
    if (level == nullptr)
        return;

    size_t start_x = static_cast<size_t>(entities[0]->get_pos().x);
    size_t start_y = static_cast<size_t>(entities[0]->get_pos().y);

    // Track which tiles have been visited
    std::vector<std::vector<bool>> visited(static_cast<size_t>(level->get_size().y), std::vector<bool>(static_cast<size_t>(level->get_size().x), false));
    flood_fill(start_x, start_y, visited, level);

    // Set tiles that are not visited to empty
    for (size_t i = 0; i < static_cast<size_t>(level->get_size().y); i++)
    {
        for (size_t j = 0; j < static_cast<size_t>(level->get_size().x); j++)
        {
            if (!visited[i][j])
            {
                level->set_tile(j, i, {level->get_tile(j, i).depth, empty_symb});
            }
        }
    }
}
