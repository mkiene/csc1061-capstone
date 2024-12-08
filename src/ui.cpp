/*
 * Max Kiene
 * Sunday, December 8th, 2024
 * CSC1061 Capstone
 */

#include "../include/ui.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include "../include/input.h"
#include "../include/terminal.h"

Menu menu;

std::vector<ui_content> ui_draw_queue;

/*
 * Calculates the visible width of a string after removing ANSI escape sequences.
 * Returns the length of the cleaned string.
 */

int calculate_visible_width(const std::string &content)
{
    std::regex ansi_escape("\033\\[[0-9;]*m");
    std::string cleaned_content = std::regex_replace(content, ansi_escape, "");
    return static_cast<int>(cleaned_content.length());
}

/*
 * Adds a UI element to the draw queue to be rendered later.
 * The element is defined by its x and y position and its content.
 */

void draw_ui_content(int x, int y, const std::string &content)
{
    ui_draw_queue.push_back({x, y, content});
}

/*
 * Renders all queued UI elements to the terminal and clears the draw queue.
 * Each element is placed at its specified position using ANSI escape codes.
 */

void draw_ui()
{
    std::stringstream ui_stream;

    for (const auto &i : ui_draw_queue)
    {
        ui_stream << "\033[" << i.y << ";" << i.x << "H" << i.content;
    }
    ui_draw_queue.clear();
    std::cout << ui_stream.str();
    std::cout.flush();
}

/*
 * Sets the cursor position in the terminal to the specified x and y coordinates.
 * This is achieved using ANSI escape codes.
 */

void set_cursor_position(int x, int y)
{
    std::cout << "\033[" << y << ";" << x << "H";
}

/*
 * Reads a file and prints its contents to the terminal at a specified x and y position.
 * If the file cannot be opened, an error message is displayed.
 */

void print_file_at(const std::string &relative_path, int x, int y)
{
    std::ifstream file(relative_path);
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file " << relative_path << std::endl;
        return;
    }

    std::string line;
    int current_y = y;
    while (std::getline(file, line))
    {
        set_cursor_position(x, current_y);
        std::cout << line;
        current_y++;
    }

    file.close();
    std::cout << std::endl;
}

/*
 * Determines the width and height of a text file based on its longest line and total line count.
 * Returns a pair containing the width and height.
 */

std::pair<int, int> get_dimensions(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return {0, 0};
    }

    int width = 0;
    int height = 0;

    std::string line;
    while (std::getline(file, line))
    {
        width = std::max(width, static_cast<int>(line.length()));
        height++;
    }

    file.close();
    return {width, height};
}

/*
 * Displays the title screen menu.
 * Renders ASCII art from a file and displays menu options centered on the screen.
 */

std::string base_path = std::filesystem::current_path().string() + "/assets/ascii_art/";  // Resolve at runtime

void Menu::trigger_titlescreen()
{
    int tcx = static_cast<int>(term.size.x / 2.0f);
    int tcy = static_cast<int>(term.size.y / 2.0f);

    std::string title_art_path = base_path + "titlescreen.txt";

    auto dimensions = get_dimensions(title_art_path);
    print_file_at(title_art_path, (static_cast<int>(term.size.x) - dimensions.first) / 2, (static_cast<int>(term.size.y) - dimensions.second) / 2 - 15);

    std::string start_button = "[ESC] Resume";
    draw_ui_content(tcx - calculate_visible_width(start_button) / 2, tcy - 5, start_button);

    std::string controls_button = "[2] Controls";
    draw_ui_content(tcx - calculate_visible_width(controls_button) / 2, tcy - 3, controls_button);

    std::string readme_button = "[3] Quit";
    draw_ui_content(tcx - calculate_visible_width(readme_button) / 2, tcy - 1, readme_button);
}

/*
 * Displays the controls menu.
 * Clears the screen and prints key mappings for controlling the game.
 */

void Menu::trigger_controls()
{
    clear_screen();

    std::cout << "Controls\n\n"

              << "k/w: Move Up\n"
              << "h/a: Move Left\n"
              << "j/s: Move Down\n"
              << "l/d: Move Right\n\n"

              << "ESC: Pause Demo\n"
              << "Q: Quit Demo\n"
              << "n: Restart Demo\n"
              << "t: Toggle Lighting\n"
              << "r: Recalibrate Display"

              << std::endl;
}

/*
 * Sets the input character to 'Q' to indicate that the game should exit.
 */

void Menu::exit_game()
{
    inp = 'Q';
}

/*
 * Updates the menu based on the current state.
 * Triggers the appropriate menu screen, such as the title screen.
 */

void Menu::update()
{
    switch (state)
    {
        case 1:  // titlescreen
            trigger_titlescreen();
            break;
    }
}
