/*
 * Max Kiene
 * Sunday, December 8th, 2024
 * CSC1061 Capstone
 */

#pragma once

#include <vector>

#include "tile.h"
#include "utils.h"

/*
 * Level class.
 * This represents a playable area in the game, defined by its origin (pos),
 * size, and rendering depth. The level consists of a 2D matrix of Tile
 * objects, each of which has its own depth and symbol (see tile.h/cpp).
 * The `tiles` matrix is initialized with default Tile objects.
 */

class Level
{
   private:
    const vec2 pos;
    const vec2 size;
    const int depth;
    std::vector<std::vector<Tile>> tiles;

   public:
    Level(const vec2 pos = vec2(0.0f, 0.0f), const vec2 size = vec2(300.0f, 300.0f), const int depth = 0) : pos(pos), size(size), depth(depth), tiles(static_cast<size_t>(size.y), std::vector<Tile>(static_cast<size_t>(size.x), Tile())){};

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

    /*
     * Template function for retrieving a Tile at (x, y).
     * This function ensures that the indices are integers by casting the input type (T) to int.
     * If the indices are out of bounds, it throws an std::out_of_range exception.
     * Note: Non-integer values (e.g., float) are truncated during the cast.
     */

    template <typename T>
    Tile get_tile(T x, T y) const
    {
        int ix = static_cast<int>(x);
        int iy = static_cast<int>(y);

        if (ix < 0 || ix >= static_cast<int>(size.x) || iy < 0 || iy >= static_cast<int>(size.y))
        {
            throw std::out_of_range("get_tile: index out of bounds");
        }
        return tiles[y][x];
    }

    /*
     * Template function for setting/changing a Tile at (x, y).
     * Similar to get_tile. this function ensures indices are integers by casting the input type (T) to int.
     * If the indices are out of bounds, it throws an std::out_of_range exception.
     */

    template <typename T>
    void set_tile(T x, T y, Tile new_tile)
    {
        int ix = static_cast<int>(x);
        int iy = static_cast<int>(y);

        if (ix < 0 || ix >= static_cast<int>(size.x) || iy < 0 || iy >= static_cast<int>(size.y))
        {
            throw std::out_of_range("set_tile: index out of bounds");
        }
        tiles[y][x] = new_tile;
    }
};

extern std::vector<std::shared_ptr<Level>> levels;  // Declare the global level list
