#include "../include/config.h"

// Lighting
const int torch_radius = 10;    // Unapplied
int current_torch_radius = 10;  // unapplied
int torch_flicker_amount = 1;   // unapplied
int torch_flicker_rate = 1;     // unapplied

// RGB
bool rgb_mode = 0;                     // unapplied
float max_color_mult_for_dimming = 1;  // unapplied

// Viewport
bool clamp_viewport_to_level_bounds = 1;  // unapplied
bool viewport_follow = 1;                 // unapplied
float viewport_follow_speed = 1;          // unapplied
int viewport_slack_x = 1;                 // unapplied
int viewport_slack_y = 1;                 // unapplied
int x_scaling = 2;                        // unapplied

// Game
const char wall_symb = '#';    // unapplied
const char empty_symb = ' ';   // unapplied
const char player_symb = '@';  // unapplied
const char floor_symb = '.';   // unapplied

int view_radius;  // unapplied
