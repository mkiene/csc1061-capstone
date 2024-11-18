#pragma once

#include <memory>
#include <vector>

#include "entity.h"
#include "level.h"
#include "terminal.h"
#include "utils.h"

class Viewport
{
   public:
    vec2 level_pos;
    vec2 term_pos;
    vec2 size;

    std::shared_ptr<Level> level;

    Viewport(vec2 level_pos = {0.0f, 0.0f}, vec2 term_pos = {0.0f, 0.0f}, vec2 size = {0.0f, 0.0f}, std::shared_ptr<Level> level = nullptr) : level_pos(level_pos), term_pos(term_pos), size(size), level(level){};

    void init();
    void draw();
    void move(Entity& target);
};

extern std::vector<std::shared_ptr<Viewport>> viewports;
