#pragma once

#include <memory>
#include <vector>

#include "tile.h"
#include "utils.h"

// Level object. This determines the origin and size of the playable area, as well as a 2d matrix of Tile objects which have a depth and a symbol (char).

class Level
{
   private:
    const vec2 pos;                        // World origin (immutable)
    const vec2 size;                       // World size (immutable)
    const int depth;                       // Draw depth
    std::vector<std::vector<Tile>> tiles;  // 2d matrix of Tile objects. See tile.h/cpp

   public:
    Level(const vec2 pos = {0.0f, 0.0f}, const vec2 size = {300.0f, 300.0f}, const int depth = 0) : pos(pos), size(size), depth(depth), tiles(size.y, std::vector<Tile>(size.x, Tile()))
    {
    }  // Constructor with default params

    vec2 get_pos() const
    {
        return pos;
    }

    vec2 get_size() const
    {
        return size;
    }

    int get_depth() const
    {
        return depth;
    }

    Tile get_tile(int x, int y) const
    {
        if (x < 0 || x >= size.x || y < 0 || y >= size.y)
        {
            throw std::out_of_range("get_tile: index out of bounds");
        }
        return tiles[y][x];
    }

    void set_tile(int x, int y, Tile new_tile)
    {
        if (x < 0 || x >= size.x || y < 0 || y >= size.y)
        {
            throw std::out_of_range("set_tile: index out of bounds");
        }
        tiles[y][x] = new_tile;
    }
};

extern std::vector<std::shared_ptr<Level>> levels;  // Declare the global level list
