#pragma once

#include <vector>

#include "entity.h"
#include "terminal.h"
#include "viewport.h"

// Terminal
extern Terminal term;

// Lighting
extern const int torch_radius;
extern int current_torch_radius;
extern int torch_flicker_amount;
extern int torch_flicker_rate;

// RGB
extern bool rgb_mode;
extern float max_color_mult_for_dimming;

// Viewport
extern bool clamp_viewport_to_level_bounds;
extern bool viewport_follow;
extern float viewport_follow_speed;
extern int viewport_slack_x;
extern int viewport_slack_y;
extern int x_scaling;

// Game
extern int level_width;
extern int level_height;
extern const char wall_symb;
extern const char empty_symb;
extern const char player_symb;
extern const char floor_symb;
extern int view_radius;
