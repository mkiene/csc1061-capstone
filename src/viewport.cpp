
#include "../include/viewport.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ranges>
#include <sstream>
#include <utility>
#include <vector>

#include "../include/config.h"

std::vector<std::shared_ptr<Viewport>> viewports;

// Initialize the viewport
void Viewport::init()
{
    size.x = std::min((term.size.x / x_scaling) - 10, size.x);
    size.y = std::min(term.size.y - 10, size.y);

    term_pos = {(term.size.x / (2 * x_scaling)) - (size.x / 2), (term.size.y / 2) - (size.y / 2)};
}

/*
Draw the contents of the viewport to the terminal.

Using a stringstream, we loop through the level tiles corresponding to the viewport
visible area, and through the entity objects list. These are gathered into a draw
queue, which is then sorted based on depth. This happens for every tile within the
viewport.
*/

void Viewport::draw()
{
    std::stringstream ss;

    for (int i = 0; i < size.y; i++)
    {
        for (int j = 0; j < size.x; j++)
        {
            int draw_term_x = (int)term_pos.x + j;  // Current tile position, relative to terminal
            int draw_term_y = (int)term_pos.y + i;

            int draw_level_x = (int)level_pos.x + j;  // Current tile position, relative to level
            int draw_level_y = (int)level_pos.y + i;

            std::vector<std::pair<int, char>> draw_queue;  // Stores gathered objects

            if (draw_level_x < level->size.x && draw_level_x >= 0 && draw_level_y < level->size.y && draw_level_y >= 0)
            {
                draw_queue.push_back(std::make_pair(level->tiles[draw_level_y][draw_level_x].depth, level->tiles[draw_level_y][draw_level_x].symbol));  // Add visible level tile to the queue

                for (auto entity : entities)
                {
                    if ((int)entity->pos.x == draw_level_x && (int)entity->pos.y == draw_level_y)
                    {
                        draw_queue.push_back(std::make_pair(entity->depth, entity->symbol));  // Add visible entity to the queue
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

    std::cout << ss.str();  // Output the stringstream.
    std::cout.flush();      // Apply the new frame
}

void Viewport::move(Entity& target)  // Makes the viewport follow a target.
{
    vec2 viewport_center = {level_pos.x + size.x / 2, level_pos.y + size.y / 2};

    if (target.pos.x - viewport_center.x > 5.0f)
    {
        level_pos.x += 1.0f;
    }

    if (target.pos.x - viewport_center.x < -5.0f)
    {
        level_pos.x -= 1.0f;
    }

    if (target.pos.y - viewport_center.y > 5.0f)
    {
        level_pos.y += 1.0f;
    }

    if (target.pos.y - viewport_center.y < -5.0f)
    {
        level_pos.y -= 1.0f;
    }
}
