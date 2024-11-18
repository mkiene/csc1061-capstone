#include "../include/entity.h"

#include "../include/utils.h"

std::vector<std::shared_ptr<Entity>> entities;

void Entity::init()
{
}

void Entity::update()
{
}

void Entity::move(int direction)  // Attempt to move, checking for collisions
{
    vec2 next_pos;  // Potential future position

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

    if (next_pos.x < level->size.x && next_pos.x >= 0 && next_pos.y < level->size.y && next_pos.y >= 0)  // collision check
    {
        if (level->tiles[(int)next_pos.y][(int)next_pos.x].depth != depth)  // Only collides with entites at the same depth level
        {
            pos = next_pos;  // If the move is valid, apply the next position to the current
        }
    }
}
