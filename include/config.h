#pragma once

#include <vector>

#include "entity.h"
#include "terminal.h"
#include "viewport.h"

// Terminal
extern Terminal term;  // Global terminal

// Lighting
extern const int torch_radius;    // Unapplied
extern int current_torch_radius;  // Unapplied
extern int torch_flicker_amount;  // Unapplied
extern int torch_flicker_rate;    // Unapplied

// RGB
extern bool rgb_mode;                     // Unapplied
extern float max_color_mult_for_dimming;  // Unapplied

// Viewport
extern bool clamp_viewport_to_level_bounds;  // Unapplied
extern bool viewport_follow;                 // Unapplied
extern float viewport_follow_speed;          // Unapplied
extern int viewport_slack_x;                 // Unapplied
extern int viewport_slack_y;                 // Unapplied
extern int x_scaling;                        // Unapplied

// Game
extern int level_width;         // Unapplied
extern int level_height;        // Unapplied
extern const char wall_symb;    // Unapplied
extern const char empty_symb;   // Unapplied
extern const char player_symb;  // Unapplied
extern const char floor_symb;   // Unapplied
extern int view_radius;         // Unapplied
