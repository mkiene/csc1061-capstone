/*
 * Max Kiene
 * Sunday, December 8th, 2024
 * CSC1061 Capstone
 */

#include "../include/entity.h"

#include "../include/config.h"
#include "../include/game.h"

std::vector<std::shared_ptr<Entity>> entities;
std::vector<std::shared_ptr<Monster>> monsters;

/*
 * Implementation of Player move function.
 * Takes an integer to specify intended move direction, calculates next position,
 * and only applies if the next position is valid (i.e. not an empty space,
 * out of bounds, or a tile of the same depth).
 */

void Player::move(int direction)
{
    if (game_paused)
        return;

    vec2 next_pos;

    switch (direction)
    {
        case 0:  // left
            next_pos = vec2(this->get_pos().x - 1.0f, this->get_pos().y);
            break;
        case 1:  // up
            next_pos = vec2(this->get_pos().x, this->get_pos().y - 1.0f);
            break;
        case 2:  // right
            next_pos = vec2(this->get_pos().x + 1.0f, this->get_pos().y);
            break;
        case 3:  // down
            next_pos = vec2(this->get_pos().x, this->get_pos().y + 1.0f);
            break;
    }

    if (level->get_tile(next_pos.x, next_pos.y).symbol == ' ')
        return;

    if (next_pos.x < level->get_size().x && next_pos.x > 0 && next_pos.y < level->get_size().y && next_pos.y > 0)
    {
        if (level->get_tile(next_pos.x, next_pos.y).depth != this->get_depth())
        {
            this->set_pos(next_pos);
        }
    }
}

/*
 * Implementation of Monster move function.
 *
 * If the monster is in the player's line of sight (illuminated by the torch), seek the player:
 * 1. Calculate relative direction
 * 2. Calculate next position based on speed (configurable)
 * 3. If the next position is within a certain range of another monster, apply a separation factor
 * 4. If the next position is valid, apply new position
 */

void Monster::move(int direction)
{
    if (game_paused || target == nullptr || level == nullptr)
        return;

    if (!in_line_of_sight(target->get_pos(), this->get_pos(), empty_symb))
        return;

    vec2 dir = target->get_pos() - this->get_pos();

    if (dir.magnitude() < 1.0f)
        return;

    vec2 next_pos = this->get_pos() + dir.normalize() * this->get_speed();

    for (auto i : monsters)
    {
        if (i.get() == this)
            continue;

        if ((next_pos - i->get_pos()).magnitude() < 1.0f)  // Collide with other monsters
        {
            vec2 sep = next_pos - i->get_pos();
            next_pos = next_pos + sep.normalize();
        }
    }

    if (level->get_tile(next_pos.x, next_pos.y).symbol != empty_symb)
        this->set_pos(next_pos);
}

/*
 * Function for spawning Monsters throughout the level
 *
 * For every tile in the level:
 * 1. If the tile is a floor tile, generate a random number
 * 2. If that number is <= a threshold (spawn_chance), then create the Monster
 */

void mass_spawn(std::shared_ptr<Level> level, float spawn_chance)
{
    if (level == nullptr)
        return;

    for (int i = 0; i < static_cast<int>(level->get_size().x); i++)
    {
        for (int j = 0; j < static_cast<int>(level->get_size().y); j++)
        {
            if (level->get_tile(i, j).symbol == floor_symb && get_random_float(0.0f, 1.0f) <= spawn_chance)
            {
                auto new_monster = std::make_shared<Monster>();
                new_monster->set_pos(vec2(j, i));
                new_monster->set_level(level);
                new_monster->set_target(entities[0]);
                new_monster->set_speed(0.08f);
                monsters.push_back(new_monster);
            }
        }
    }
}
