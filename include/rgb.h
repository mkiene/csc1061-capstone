#pragma once
#include <string>
#include <vector>

// Unapplied

struct rgb
{
    int r, g, b;
};

extern rgb default_color;
extern rgb player_color;
extern rgb wall_color;
extern rgb floor_color;

std::string get_color(rgb color);

rgb normalize_color(float sqr_dist, float max_radius, rgb color);

extern std::vector<std::vector<rgb>> color_map;
