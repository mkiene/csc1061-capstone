#include "../include/rgb.h"

#include <algorithm>

#include "../include/config.h"

std::string get_color(rgb color)  // returns an ANSI escape sequence for drawing a color
{
    return "\033[38;2;" + std::to_string(color.r) + ";" + std::to_string(color.g) + ";" + std::to_string(color.b) + "m";
}

rgb normalize_color(float sqr_dist, float max_radius, rgb color)  // Returns an adjusted RGB color based on distance from player
{
    rgb new_color = color;
    // Compute the distance ratio squared (squared to ensure non-linear falloff)
    float distance_ratio = sqr_dist / (max_radius * max_radius);

    // Clamp the ratio between 0 and 1
    distance_ratio = std::clamp(distance_ratio, 0.0f, max_color_mult_for_dimming);

    // Calculate the scaling factor: inverse of distance ratio
    float scale = 1.0f - distance_ratio;

    // Apply the scale to the original color
    new_color.r = std::max(0.0f, std::min(255.0f, color.r * scale));
    new_color.g = std::max(0.0f, std::min(255.0f, color.g * scale));
    new_color.b = std::max(0.0f, std::min(255.0f, color.b * scale));

    return new_color;
}
