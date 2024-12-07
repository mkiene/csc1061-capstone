#include "../include/ui.h"

#include <filesystem>
#include <fstream>
#include <iostream>  // For console output
#include <regex>
#include <string>

#include "../include/game.h"
#include "../include/viewport.h"

Menu menu;

// Queue for storing UI elements
std::vector<ui_content> ui_draw_queue;

int calculate_visible_width(const std::string &content)
{
    // Regular expression to match ANSI escape sequences
    std::regex ansi_escape("\033\\[[0-9;]*m");

    // Remove ANSI escape sequences
    std::string cleaned_content = std::regex_replace(content, ansi_escape, "");

    // Return the length of the cleaned string
    return cleaned_content.length();
}

// Add a UI element to the draw queue
void draw_ui_content(int x, int y, const std::string &content)
{
    ui_draw_queue.push_back({x, y, content});
}

void draw_ui()  // Draw ui content at x and y.
{
    std::stringstream ui_stream;

    for (const auto &i : ui_draw_queue)
    {
        ui_stream << "\033[" << i.y << ";" << i.x << "H" << i.content;
    }
    ui_draw_queue.clear();         // Clear the queue after processing all elements
    std::cout << ui_stream.str();  // Output the stringstream
    std::cout.flush();             // Apply the new frame
}

void set_cursor_position(int x, int y)
{
    // ANSI escape code to set the cursor position
    std::cout << "\033[" << y << ";" << x << "H";
}

void print_file_at(const std::string &relative_path, int x, int y)
{
    std::ifstream file(relative_path);  // Now relative to the project root
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file " << relative_path << std::endl;
        return;
    }

    std::string line;
    int current_y = y;  // Start at the specified y position
    while (std::getline(file, line))
    {
        set_cursor_position(x, current_y);  // Move to the desired x, y position
        std::cout << line;
        current_y++;  // Move to the next line
    }

    file.close();
    // Reset the cursor to a new line after printing to avoid clutter
    std::cout << std::endl;
}

std::pair<int, int> get_dimensions(const std::string &filename)
{
    std::ifstream file(filename);  // Open the file
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return {0, 0};  // Return zero dimensions if the file can't be opened
    }

    int width = 0;   // Track the maximum line length
    int height = 0;  // Count the number of lines

    std::string line;
    while (std::getline(file, line))
    {
        width = std::max(width, static_cast<int>(line.length()));  // Update max width
        height++;                                                  // Increment the line count
    }

    file.close();
    return {width, height};  // Return dimensions as a pair
}

void Menu::trigger_titlescreen()
{
    int tcx = term.size.x / 2;
    int tcy = term.size.y / 2;

    // display header art
    auto dimensions = get_dimensions("/Users/max/CSC1061-Capstone/assets/ascii_art/titlescreen.txt");
    print_file_at("/Users/max/CSC1061-Capstone/assets/ascii_art/titlescreen.txt", (term.size.x - dimensions.first) / 2, (term.size.y - dimensions.second) / 2 - 15);

    std::string start_button = "[1] Start";
    draw_ui_content(tcx - calculate_visible_width(start_button) / 2, tcy - 5, start_button);

    std::string controls_button = "[2] Controls";
    draw_ui_content(tcx - calculate_visible_width(controls_button) / 2, tcy - 3, controls_button);

    std::string readme_button = "[3] README";
    draw_ui_content(tcx - calculate_visible_width(readme_button) / 2, tcy - 1, readme_button);
}

void Menu::trigger_controls()
{
    clear_screen();

    std::cout << "Hello, World!" << std::endl;
}

void Menu::update()
{
    switch (state)
    {
        case 1:  // titlescreen
            trigger_titlescreen();
            break;
    }
}
