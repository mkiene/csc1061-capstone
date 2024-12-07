#pragma once

#include <cmath>
#include <vector>

int get_random_int(int min, int max);
bool in_line_of_sight(int x1, int y1, int x2, int y2, char blocker);
int sqr_dist(int x1, int y1, int x2, int y2);
void clear_screen();

struct vec2  // Custom 2d vector implementation
{
    float x;
    float y;

    vec2() = default;
    vec2(float x, float y) : x(x), y(y)
    {
    }

    vec2 operator+(const vec2 &other) const
    {
        return {x + other.x, y + other.y};
    }

    vec2 operator-(const vec2 &other) const
    {
        return {x - other.x, y - other.y};
    }

    vec2 operator*(float scalar) const
    {
        return {x * scalar, y * scalar};
    }

    vec2 operator/(float scalar) const
    {
        return {x / scalar, y / scalar};
    }
    float magnitude() const
    {
        return std::sqrt(x * x + y * y);
    }

    vec2 normalize() const
    {
        float mag = magnitude();
        return {x / mag, y / mag};
    }

    vec2 sign() const
    {
        return {float(x > 0) - (x < 0),  // 1 if positive, -1 if negative, 0 if zero
                float(y > 0) - (y < 0)};
    }
};

template <typename T>
std::vector<std::vector<std::vector<T>>> init_3d_tiles(size_t cols, size_t rows, size_t stack_size, const T &default_value)  // Unapplied
{
    std::vector<std::vector<std::vector<T>>> tiles;

    tiles.resize(cols);
    for (size_t c = 0; c < cols; c++)
    {
        tiles[c].resize(rows);
        for (size_t r = 0; r < rows; r++)
        {
            tiles[c][r].resize(stack_size, default_value);
        }
    }

    return tiles;
}
