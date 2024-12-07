#pragma once

#include "utils.h"

struct Room
{
    vec2 pos;
    vec2 size;
    int id;
};

extern std::vector<std::shared_ptr<Room>> rooms;
