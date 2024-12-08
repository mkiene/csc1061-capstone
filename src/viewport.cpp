/*
 * Max Kiene
 * Sunday, December 8th, 2024
 * CSC1061 Capstone
 */

#include "../include/viewport.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

#include "../include/config.h"
#include "../include/game.h"
#include "../include/terminal.h"

std::vector<std::shared_ptr<Viewport>> viewports;

/*
 * Initializes the viewport by setting its size and position within the terminal.
 * Ensures the viewport dimensions fit within the terminal boundaries.
 */

void Viewport::init()
{
    term.init();

    size.x = std::min((term.size.x / static_cast<float>(x_scaling)) - static_cast<float>(terminal_border_padding), size.x);
    size.y = std::min(term.size.y - static_cast<float>(terminal_border_padding), size.y);

    term_pos = vec2((term.size.x / (2.0f * static_cast<float>(x_scaling))) - (size.x / 2), (term.size.y / 2) - (size.y / 2));
}

/*
 * Draws the viewport to the terminal.
 * Displays tiles, entities, and monsters within the viewport boundaries, considering depth and visibility.
 */

void Viewport::draw() const
{
    if (game_paused || level == nullptr)
        return;

    std::stringstream ss;

    for (int i = 0; i < static_cast<int>(size.y); i++)
    {
        for (int j = 0; j < static_cast<int>(size.x); j++)
        {
            int draw_term_x = static_cast<int>(term_pos.x) + j;
            int draw_term_y = static_cast<int>(term_pos.y) + i;

            if (draw_viewport_edges)
            {
                if (i == 0 || i == static_cast<int>(size.y) - 1)
                {
                    for (int k = 0; k < x_scaling; k++)
                    {
                        ss << "\033[" << draw_term_y << ";" << (draw_term_x * x_scaling) + k << "H" << '-' << "\033[0m";
                        if (j == static_cast<int>(size.x) - 1)
                            k = x_scaling - 1;
                    }
                    continue;
                }
                if (j == 0 || j == static_cast<int>(size.x) - 1)
                {
                    ss << "\033[" << draw_term_y << ";" << draw_term_x * x_scaling << "H" << '|' << "\033[0m";
                    continue;
                }
            }

            vec2 draw_pos(static_cast<int>(level_pos.x) + j, static_cast<int>(level_pos.y) + i);

            std::vector<std::pair<int, char>> draw_queue;

            if (draw_pos > vec2(0.0f, 0.0f) && draw_pos < level->get_size())
            {
                if (enable_LOS == false || in_line_of_sight(entities[0]->get_pos(), draw_pos, empty_symb))
                {
                    draw_queue.push_back(std::make_pair(level->get_tile(draw_pos.x, draw_pos.y).depth, level->get_tile(draw_pos.x, draw_pos.y).symbol));

                    for (auto entity : entities)
                    {
                        if (entity->get_pos().rounded() == draw_pos.rounded())
                        {
                            draw_queue.push_back(std::make_pair(entity->get_depth(), entity->get_symbol()));
                        }
                    }

                    for (auto monster : monsters)
                    {
                        if (monster->get_pos().rounded() == draw_pos.rounded())
                        {
                            draw_queue.push_back(std::make_pair(monster->get_depth(), monster->get_symbol()));
                        }
                    }
                }
            }

            std::sort(draw_queue.begin(), draw_queue.end(), [](const std::pair<int, char>& a, const std::pair<int, char>& b) { return a.first < b.first; });

            char to_draw = ' ';

            if (!draw_queue.empty())
            {
                to_draw = draw_queue[0].second;
            }

            ss << "\033[" << draw_term_y << ";" << draw_term_x * x_scaling << "H" << to_draw << "\033[0m";
        }
    }

    std::cout << ss.str();
    std::cout.flush();
}

/*
 * Moves the viewport to keep the target entity within the slack bounds.
 * Adjusts the viewport position while ensuring it stays within level boundaries.
 */

void Viewport::move()
{
    if (game_paused || target == nullptr || !viewport_follow)
        return;

    vec2 viewport_center(level_pos.x + size.x / 2, level_pos.y + size.y / 2);

    if (target->get_pos().x - viewport_center.x > static_cast<float>(viewport_slack_x))
    {
        if (clamp_viewport_to_level_bounds && level_pos.x + size.x < level->get_size().x)
            level_pos.x += 1.0f;
    }

    if (target->get_pos().x - viewport_center.x < -static_cast<float>(viewport_slack_x))
    {
        if (clamp_viewport_to_level_bounds && level_pos.x > 0)
            level_pos.x -= 1.0f;
    }

    if (target->get_pos().y - viewport_center.y > static_cast<float>(viewport_slack_y))
    {
        if (clamp_viewport_to_level_bounds && level_pos.y + size.y < level->get_size().x)
            level_pos.y += 1.0f;
    }

    if (target->get_pos().y - viewport_center.y < -static_cast<float>(viewport_slack_y))
    {
        if (clamp_viewport_to_level_bounds && level_pos.y > 0)
            level_pos.y -= 1.0f;
    }
}

/*
 * Updates the viewport by moving and redrawing it.
 * Skips updates if the game is paused.
 */

void Viewport::update()
{
    if (game_paused)
        return;

    move();
    draw();
}
