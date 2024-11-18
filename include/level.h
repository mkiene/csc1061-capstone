#pragma once

#include <vector>

#include "tile.h"
#include "utils.h"

class Level
{
   public:
    vec2 pos;
    vec2 size;
    int depth;
    std::vector<std::vector<Tile>> tiles;

    Level(vec2 pos = {0.0f, 0.0f}, vec2 size = {300.0f, 300.0f}, int depth = 0) : pos(pos), size(size), depth(depth){};

    void init();
};

extern std::vector<std::shared_ptr<Level>> levels;
