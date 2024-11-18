#include "../include/config.h"

// Lighting
const int torch_radius = 10;
int current_torch_radius = 10;
int torch_flicker_amount = 1;
int torch_flicker_rate = 1;

// RGB
bool rgb_mode = 0;
float max_color_mult_for_dimming = 1;

// Viewport
bool clamp_viewport_to_level_bounds = 1;
bool viewport_follow = 1;
float viewport_follow_speed = 1;
int viewport_slack_x = 1;
int viewport_slack_y = 1;
int x_scaling = 2;

// Game
const char wall_symb = '#';
const char empty_symb = ' ';
const char player_symb = '@';
const char floor_symb = '.';

int view_radius;
