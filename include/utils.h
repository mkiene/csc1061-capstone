/*
 * Max Kiene
 * Sunday, December 8th, 2024
 * CSC1061 Capstone
 */

#pragma once

#include <random>

/*
 * Custom vec2 implementation.
 *
 * Represents a vector of real numbers, and supports the following vector operations:
 * - Addition, subtraction, multiplication, and division
 * - Comparing (==, <(=) >(=))
 * - magnitude(): Returns the length of the vector
 * - normalize(): Scales the x and y components to be values between 0 and 1
 * - rounded(): Rounds the x and y components
 * - sign(): Evaluates the sign of the x and y components, returning -1, 0, or 1
 */

struct vec2
{
    float x;
    float y;

    vec2() = default;

    template <typename T>
    explicit vec2(T x, T y) : x(static_cast<float>(x)), y(static_cast<float>(y))
    {
    }

    vec2 operator+(const vec2 &other) const
    {
        return vec2(x + other.x, y + other.y);
    }

    vec2 operator-(const vec2 &other) const
    {
        return vec2(x - other.x, y - other.y);
    }

    vec2 operator*(float scalar) const
    {
        return vec2(x * scalar, y * scalar);
    }

    vec2 operator/(float scalar) const
    {
        return vec2(x / scalar, y / scalar);
    }

    bool operator==(const vec2 &other) const
    {
        return (x == other.x) && (y == other.y);
    }

    bool operator<(const vec2 &other) const
    {
        return (x < other.x) && (y < other.y);
    }

    bool operator<=(const vec2 &other) const
    {
        return (x <= other.x) && (y <= other.y);
    }

    bool operator>(const vec2 &other) const
    {
        return (x > other.x) && (y > other.y);
    }

    bool operator>=(const vec2 &other) const
    {
        return (x >= other.x) && (y >= other.y);
    }

    float magnitude() const
    {
        return abs(std::sqrt(x * x + y * y));
    }

    vec2 normalize() const
    {
        float mag = magnitude();
        return vec2(x / mag, y / mag);
    }

    vec2 rounded() const
    {
        return vec2(std::round(x), std::round(y));
    }

    vec2 sign() const
    {
        return vec2((x > 0.0f) - (x < 0.0f), (y > 0.0f) - (y < 0.0f));  // 1 if positive, -1 if negative, 0 if zero
    }
};

// Template function for returning a random integer between min and max.

template <typename T>
int get_random_int(T min, T max)
{
    int imin = static_cast<int>(min);
    int imax = static_cast<int>(max);

    static std::random_device dev;
    static std::mt19937 rng(dev());
    std::uniform_int_distribution<int> dist(imin, imax);
    return dist(rng);
}

// Template function for returning a random real number between min and max.

template <typename T>
float get_random_float(T min, T max)
{
    float fmin = static_cast<float>(min);
    float fmax = static_cast<float>(max);

    static std::random_device dev;
    static std::mt19937 rng(dev());
    std::uniform_real_distribution<float> dist(fmin, fmax);
    return dist(rng);
}

bool in_line_of_sight(vec2 start, vec2 end, char blocker);
float sqr_dist(float x1, float y1, float x2, float y2);
void clear_screen();
