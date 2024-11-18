#pragma once

#include <memory>
#include <vector>

#include "entity.h"
#include "level.h"
#include "terminal.h"
#include "utils.h"

/*
The Viewport handles the display of the game onto the screen. You can think of it
as the game's 'camera'.

A few important points:

   -  The viewport has two positions: 'level_pos', which is relative to the level's
      origin (Level::pos), and 'term_pos', which is relative to the terminal window.

   -  The viewport shows only a portion of the level. The level can be theoretically
      infinitely big.

   -  The viewport renders 'depth' by looping through each visible level tile and
      entity, storing their corresponding depths to a buffer. Then, this buffer is
      sorted, and only the object with the least depth is drawn. This effectively
      simulates three dimensionality (albeit top-down).

   -  There can be potentially multiple viewports per terminal, but this project
      currently only utilizes one.

   -  The viewport is very efficient, since the only tiles which the game needs to
      worry about are the ones immediately visible through the viewport, which can
      be directly calculated instead of looping through every tile.
*/

class Viewport
{
   public:
    vec2 level_pos;  // position within the level
    vec2 term_pos;   // position within the terminal
    vec2 size;       // dimensions on the terminal

    std::shared_ptr<Level> level;  // The level to display

    Viewport(vec2 level_pos = {0.0f, 0.0f}, vec2 term_pos = {0.0f, 0.0f}, vec2 size = {0.0f, 0.0f}, std::shared_ptr<Level> level = nullptr) : level_pos(level_pos), term_pos(term_pos), size(size), level(level){};  // Constructor

    void init();                // Initialize the viewport
    void draw();                // Draw the contents of the level that are visible through the viewport to the terminal
    void move(Entity& target);  // What the viewport should potentially follow
};

extern std::vector<std::shared_ptr<Viewport>> viewports;  // Global viewports list
