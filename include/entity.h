/*
 * Max Kiene
 * Sunday, December 8th, 2024
 * CSC1061 Capstone
 */

#pragma once

#include <memory>
#include <vector>

#include "level.h"

/*
 * Entity abstract class (Requirement 1)
 * Provides a structure for all entites.
 */

class Entity
{
   private:
    vec2 pos;
    char symbol;
    int depth;

   protected:
    std::shared_ptr<Level> level;

   public:
    Entity(vec2 pos = vec2(0.0f, 0.0f), char symbol = '?', int depth = 0, std::shared_ptr<Level> level = nullptr) : pos(pos), symbol(symbol), depth(depth), level(level){};

    // Getters
    vec2 get_pos() const
    {
        return pos;
    }

    char get_symbol() const
    {
        return symbol;
    }

    int get_depth() const
    {
        return depth;
    }

    std::shared_ptr<Level> get_level() const
    {
        return level;
    }

    // Setters
    void set_pos(const vec2& new_pos)
    {
        pos = new_pos;
    }

    void set_symbol(char new_symbol)
    {
        symbol = new_symbol;
    }

    void set_depth(int new_depth)
    {
        depth = new_depth;
    }

    void set_level(std::shared_ptr<Level> new_level)
    {
        level = new_level;
    }

    // Pure virtual function
    virtual void move(int direction) = 0;

    // Virtual destructor
    virtual ~Entity() = default;
};

/*
 * Player class, derived from Entity (requirement 2).
 * Uses polymorphism (requirement 3) to define player-specific movement behavior.
 */

class Player : public Entity
{
   public:
    Player(vec2 pos = vec2(0.0f, 0.0f), char symbol = '@', int depth = 0, std::shared_ptr<Level> level = nullptr) : Entity(pos, symbol, depth, level){};

    void move(int direction) override;
};

/*
 * Monster class, derived from Entity (requirement 2).
 * Uses polymorphism (requirement 3) to define monster-specific movement behavior.
 */

class Monster : public Entity
{
   private:
    std::shared_ptr<Entity> target;
    float speed;

   public:
    // Getters

    std::shared_ptr<Entity> get_target()
    {
        return target;
    }

    float get_speed()
    {
        return speed;
    }

    // Setters

    void set_target(std::shared_ptr<Entity> new_target)
    {
        target = new_target;
    }

    void set_speed(float new_speed)
    {
        speed = new_speed;
    }

    // Movement

    void move(int direction) override;
};

// Global vectors to store references to Entities

extern std::vector<std::shared_ptr<Entity>> entities;
extern std::vector<std::shared_ptr<Monster>> monsters;

// Spawns monsters

void mass_spawn(std::shared_ptr<Level> level, float spawn_chance);
