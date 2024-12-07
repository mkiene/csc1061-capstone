#pragma once

#include <sstream>
#include <string>
#include <vector>

// Unapplied

struct ui_content
{
    int x, y;
    std::string content;
};

extern std::vector<ui_content> ui_draw_queue;

void draw_ui();

template <typename... Args>
void draw_ui_content(int x, int y, Args &&...args)  // Allows n arguments, and different datatypes!
{
    std::stringstream ss;

    // Fold expression to unpack and insert all arguments into the stringstream
    (ss << ... << args);

    // Retrieve the concatenated string
    std::string formatted_content = ss.str();

    // Create a ui_content struct and push it to the drawing queue
    ui_content item = {x, y, formatted_content};
    ui_draw_queue.push_back(item);
}

class Menu
{
   public:
    int state;
    void trigger_titlescreen();
    void trigger_controls();

    void update();
};

extern Menu menu;
