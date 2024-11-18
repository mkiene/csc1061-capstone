#pragma once

#include <memory>
#include <vector>

#include "tile.h"
#include "utils.h"

// Level object. This determines the origin and size of the playable area, as well as a 2d matrix of Tile objects which have a depth and a symbol (char).
class Level
{
   public:
    vec2 pos;                              // World origin
    vec2 size;                             // World size
    int depth;                             // Draw depth
    std::vector<std::vector<Tile>> tiles;  // 2d matrix of Tile objects. See tile.h/cpp

    Level(vec2 pos = {0.0f, 0.0f}, vec2 size = {300.0f, 300.0f}, int depth = 0) : pos(pos), size(size), depth(depth){};  // Constructor with default params

    void init();  // Initialization function
};

extern std::vector<std::shared_ptr<Level>> levels;  // Declare the global level list
