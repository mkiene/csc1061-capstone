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
   private:
    vec2 level_pos;  // Position within the level
    vec2 term_pos;   // Position within the terminal
    vec2 size;       // Dimensions on the terminal

    std::shared_ptr<Level> level;    // Current level
    std::shared_ptr<Entity> target;  // Target entity to follow

   public:
    // Constructor
    Viewport(const vec2& level_pos = {0.0f, 0.0f}, const vec2& term_pos = {0.0f, 0.0f}, const vec2& size = {0.0f, 0.0f}, std::shared_ptr<Level> level = nullptr) : level_pos(level_pos), term_pos(term_pos), size(size), level(level)
    {
    }

    // Getters
    vec2 get_level_pos() const
    {
        return level_pos;
    }
    vec2 get_term_pos() const
    {
        return term_pos;
    }
    vec2 get_size() const
    {
        return size;
    }
    std::shared_ptr<Level> get_level() const
    {
        return level;
    }
    std::shared_ptr<Entity> get_target() const
    {
        return target;
    }

    // Setters
    void set_level_pos(const vec2& new_pos)
    {
        level_pos = new_pos;
    }
    void set_term_pos(const vec2& new_pos)
    {
        term_pos = new_pos;
    }
    void set_size(const vec2& new_size)
    {
        if (new_size.x <= 0 || new_size.y <= 0)
            throw std::invalid_argument("Size must be positive");
        size = new_size;
    }
    void set_level(std::shared_ptr<Level> new_level)
    {
        level = new_level;
    }
    void set_target(std::shared_ptr<Entity> new_target)
    {
        target = new_target;
    }

    // Other

    void init();
    void draw() const;
    void move();
    void draw_line(vec2 p1, vec2 p2) const;
    void update();
};

extern std::vector<std::shared_ptr<Viewport>> viewports;  // Global viewports list
