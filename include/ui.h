/*
 * Max Kiene
 * Sunday, December 8th, 2024
 * CSC1061 Capstone
 *
 * UI and Menu System
 *
 * This file provides functionality for managing and drawing UI elements and handling
 * the game's menu system. It includes structures and functions for rendering text
 * content on the screen and managing the state of the game's menu.
 */

#pragma once

#include <sstream>

/*
 * ui_content: Represents a piece of UI text to be drawn, including its position
 * (x, y) and the content string to render.
 */

struct ui_content
{
    int x, y;
    std::string content;
};

/*
 * ui_draw_queue: UI elements to be drawn on the screen. New elements are
 * added to this queue through the draw_ui_content function.
 */

extern std::vector<ui_content> ui_draw_queue;

/*
 * draw_ui(): Renders all elements in the ui_draw_queue to the screen. This
 * function should be called after updating the draw queue to display the UI.
 */

void draw_ui();

/*
 * draw_ui_content(int x, int y, Args &&...args):
 * A template function for adding formatted content to the ui_draw_queue.
 * Combines multiple arguments into a single string using a stringstream and
 * stores the resulting content along with its position in the draw queue.
 */

template <typename... Args>
void draw_ui_content(int x, int y, Args &&...args)
{
    std::stringstream ss;

    (ss << ... << args);

    std::string formatted_content = ss.str();

    ui_content item = {x, y, formatted_content};
    ui_draw_queue.push_back(item);
}

/*
 * Menu class. Manages the game's menu system
 *
 * - state: Represents the current state of the menu
 * - trigger_titlescreen(): Triggers the title screen state
 * - trigger_controls(): Triggers the controls/help screen state
 * - exit_game(): Self-explanatory
 * - update(): Updates the menu state, including processing user input and
 *   handling transitions
 */

class Menu
{
   public:
    int state;
    void trigger_titlescreen();
    void trigger_controls();
    void exit_game();

    void update();
};

extern Menu menu;
