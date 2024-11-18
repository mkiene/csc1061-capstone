#include "../include/ui.h"

#include <iostream>  // For console output

// Unapplied

// Queue for storing UI elements
std::vector<ui_content> ui_draw_queue;

// Add a UI element to the draw queue
void draw_ui_content(int x, int y, const std::string &content)
{
    ui_draw_queue.push_back({x, y, content});
}

void draw_ui(std::stringstream &ui_stream)  // Draw ui content at x and y.
{
    for (const auto &i : ui_draw_queue)
    {
        ui_stream << "\033[" << i.y << ";" << i.x << "H" << i.content;
    }
    ui_draw_queue.clear();  // Clear the queue after processing all elements
}
