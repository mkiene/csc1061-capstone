#include "../include/entity.h"

#include "../include/game.h"
#include "../include/utils.h"

std::vector<std::shared_ptr<Entity>> entities;

void Entity::move(int direction)  // Attempt to move, checking for collisions
{
    if (game_paused)
        return;

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

    if (level->get_tile(next_pos.x, next_pos.y).symbol == ' ')
        return;

    if (next_pos.x < level->get_size().x && next_pos.x > 0 && next_pos.y < level->get_size().y && next_pos.y > 0)  // collision check
    {
        if (level->get_tile(next_pos.x, next_pos.y).depth != depth)  // Only collides with entites at the same depth level
        {
            pos = next_pos;  // If the move is valid, apply the next position to the current
        }
    }
}
