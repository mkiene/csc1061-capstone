
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

void Viewport::init()
{
    size.x = std::min((term.size.x / x_scaling) - 10, size.x);
    size.y = std::min(term.size.y - 10, size.y);

    term_pos = {(term.size.x / (2 * x_scaling)) - (size.x / 2), (term.size.y / 2) - (size.y / 2)};
}

void Viewport::draw()
{
    std::stringstream ss;

    for (int i = 0; i < size.y; i++)
    {
        for (int j = 0; j < size.x; j++)
        {
            int draw_term_x = (int)term_pos.x + j;
            int draw_term_y = (int)term_pos.y + i;

            int draw_level_x = (int)level_pos.x + j;
            int draw_level_y = (int)level_pos.y + i;

            std::vector<std::pair<int, char>> draw_queue;

            if (draw_level_x < level->size.x && draw_level_x >= 0 && draw_level_y < level->size.y && draw_level_y >= 0)
            {
                draw_queue.push_back(std::make_pair(level->tiles[draw_level_y][draw_level_x].depth, level->tiles[draw_level_y][draw_level_x].symbol));

                for (auto entity : entities)
                {
                    if ((int)entity->pos.x == draw_level_x && (int)entity->pos.y == draw_level_y)
                    {
                        draw_queue.push_back(std::make_pair(entity->depth, entity->symbol));
                    }
                }
            }

            std::sort(draw_queue.begin(), draw_queue.end(),
                      [](const std::pair<int, char>& a, const std::pair<int, char>& b)
                      {
                          return a.first < b.first;  // Compare the int values
                      });

            char to_draw = ' ';

            if (draw_queue.size() > 0)
            {
                to_draw = draw_queue[0].second;
            }

            ss << "\033[" << draw_term_y << ";" << draw_term_x * x_scaling << "H" << to_draw << "\033[0m";
        }
    }

    std::cout << ss.str();
    std::cout.flush();
}

void Viewport::move(Entity& target)
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
