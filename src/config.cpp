/*
 * Max Kiene
 * Sunday, December 8th, 2024
 * CSC1061 Capstone
 */

#include "../include/config.h"

// Viewport
bool clamp_viewport_to_level_bounds = 1;
bool viewport_follow = 1;
bool draw_viewport_edges = 1;
int viewport_slack_x = 5;
int viewport_slack_y = 5;
int x_scaling = 2;
int terminal_border_padding = 10;

// Game
const char empty_symb = ' ';
const char floor_symb = '.';

// Lighting
bool enable_LOS = 1;
