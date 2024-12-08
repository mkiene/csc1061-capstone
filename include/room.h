/*
 * Max Kiene
 * Sunday, December 8th, 2024
 * CSC1061 Capstone
 */

#pragma once

#include "utils.h"

// Simple room struct

struct Room
{
    vec2 pos;
    vec2 size;
    int id;
};

extern std::vector<std::shared_ptr<Room>> rooms;
