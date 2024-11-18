#pragma once

#include "level.h"
#include "utils.h"

class Room
{
   public:
    vec2 pos;
    vec2 size;

    Level level;

    Room(vec2 pos = {0.0f, 0.0f}, vec2 size = {5.0f, 5.0f}) : pos(pos), size(size)
    {
    }

    void create();
};
