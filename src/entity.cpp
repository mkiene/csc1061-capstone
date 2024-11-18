#include "../include/entity.h"

#include "../include/utils.h"

std::vector<std::shared_ptr<Entity>> entities;

void Entity::init()
{
}

void Entity::update()
{
}

void Entity::move(int direction)
{
    vec2 next_pos;

    switch (direction)
    {
        case 0:  // left
            next_pos = {pos.x - 1.0f, pos.y};
            break;
        case 1:  // up
            next_pos = {pos.x, pos.y - 1.0f};
            break;
        case 2:  // right
            next_pos = {pos.x + 1.0f, pos.y};
            break;
        case 3:
            next_pos = {pos.x, pos.y + 1.0f};
            break;
    }

    if (next_pos.x < level->size.x && next_pos.x >= 0 && next_pos.y < level->size.y && next_pos.y >= 0)
    {
        if (level->tiles[next_pos.y][next_pos.x].depth != depth)
        {
            pos = next_pos;
        }
    }
}
