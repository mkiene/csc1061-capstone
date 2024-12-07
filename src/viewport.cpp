#include "../include/viewport.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

#include "../include/config.h"
#include "../include/game.h"

std::vector<std::shared_ptr<Viewport>> viewports;

// Initialize the viewport
void Viewport::init()
{
    term.init();

    size.x = std::min((term.size.x / x_scaling) - terminal_border_padding, size.x);
    size.y = std::min(term.size.y - terminal_border_padding, size.y);

    term_pos = {(term.size.x / (2 * x_scaling)) - (size.x / 2), (term.size.y / 2) - (size.y / 2)};
}

/*
Draw the contents of the viewport to the terminal.

Using a stringstream, we loop through the level tiles corresponding to the viewport
visible area, and through the entity objects list. These are gathered into a draw
queue, which is then sorted based on depth. This happens for every tile within the
viewport.
*/

void Viewport::draw() const
{
    if (game_paused || level == nullptr)
        return;

    std::stringstream ss;

    for (int i = 0; i < size.y; i++)
    {
        for (int j = 0; j < size.x; j++)
        {
            int draw_term_x = (int)term_pos.x + j;  // Current tile position, relative to terminal
            int draw_term_y = (int)term_pos.y + i;

            if (draw_viewport_edges)
            {
                if (i == 0 || i == size.y - 1)
                {
                    for (int k = 0; k < x_scaling; k++)
                    {
                        ss << "\033[" << draw_term_y << ";" << (draw_term_x * x_scaling) + k << "H" << '-' << "\033[0m";
                        if (j == size.x - 1)
                            k = x_scaling - 1;
                    }
                    continue;
                }
                if (j == 0 || j == size.x - 1)
                {
                    ss << "\033[" << draw_term_y << ";" << draw_term_x * x_scaling << "H" << '|' << "\033[0m";
                    continue;
                }
            }

            int draw_level_x = (int)level_pos.x + j;  // Current tile position, relative to level
            int draw_level_y = (int)level_pos.y + i;

            std::vector<std::pair<int, char>> draw_queue;  // Stores gathered objects

            if (draw_level_x < level->get_size().x && draw_level_x >= 0 && draw_level_y < level->get_size().y && draw_level_y >= 0)
            {
                if (enable_LOS == false || in_line_of_sight(entities[0]->get_pos().x, entities[0]->get_pos().y, draw_level_x, draw_level_y, empty_symb))
                {
                    draw_queue.push_back(std::make_pair(level->get_tile(draw_level_x, draw_level_y).depth, level->get_tile(draw_level_x, draw_level_y).symbol));  // Add visible level tile to the queue

                    for (auto entity : entities)
                    {
                        if ((int)entity->get_pos().x == draw_level_x && (int)entity->get_pos().y == draw_level_y)
                        {
                            draw_queue.push_back(std::make_pair(entity->get_depth(), entity->get_symbol()));  // Add visible entity to the queue
                        }
                    }
                }
            }

            std::sort(draw_queue.begin(), draw_queue.end(), [](const std::pair<int, char>& a, const std::pair<int, char>& b) { return a.first < b.first; });  // Sort the draw queue

            char to_draw = ' ';  // Default character to display. This covers cases where a portion of the viewport may be out of the level bounds.

            if (draw_queue.size() > 0)
            {
                to_draw = draw_queue[0].second;  // If the draw queue contains more than zero objects, display the top-most.
            }

            ss << "\033[" << draw_term_y << ";" << draw_term_x * x_scaling << "H" << to_draw << "\033[0m";  // Draw the viewport 'pixel' to the stringstream
        }
    }

    std::cout << ss.str();  // Output the stringstream
    std::cout.flush();      // Apply the new frame
}

void Viewport::move()  // Makes the viewport follow a target
{
    if (game_paused || target == nullptr || !viewport_follow)
        return;

    vec2 viewport_center = {level_pos.x + size.x / 2, level_pos.y + size.y / 2};

    if (target->get_pos().x - viewport_center.x > viewport_slack_x)
    {
        if (clamp_viewport_to_level_bounds && level_pos.x + size.x < level->get_size().x)
            level_pos.x += 1.0f;
    }

    if (target->get_pos().x - viewport_center.x < -viewport_slack_x)
    {
        if (clamp_viewport_to_level_bounds && level_pos.x > 0)
            level_pos.x -= 1.0f;
    }

    if (target->get_pos().y - viewport_center.y > viewport_slack_y)
    {
        if (clamp_viewport_to_level_bounds && level_pos.y + size.y < level->get_size().x)
            level_pos.y += 1.0f;
    }

    if (target->get_pos().y - viewport_center.y < -viewport_slack_y)
    {
        if (clamp_viewport_to_level_bounds && level_pos.y > 0)
            level_pos.y -= 1.0f;
    }
}

void Viewport::draw_line(vec2 p1, vec2 p2) const  // Bresenham's line algorithm
{
    if (game_paused)
        return;

    int x1 = (int)p1.x;
    int y1 = (int)p1.y;

    int x2 = (int)p2.x;
    int y2 = (int)p2.y;

    int dx = std::abs(x2 - x1);
    int dy = -std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx + dy, e2;  // error value e_xy

    while (true)
    {
        level->set_tile(x1, y1, {level->get_tile(x1, y1).depth, '.'});

        if (x1 == x2 && y1 == y2)
        {
            break;  // Line has reached the target
        }

        e2 = 2 * err;
        if (e2 >= dy)
        {  // e_xy + e_x > 0
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx)
        {  // e_xy + e_y < 0
            err += dx;
            y1 += sy;
        }
    }
}

void Viewport::update()
{
    if (game_paused)
        return;

    move();
    draw();
}
