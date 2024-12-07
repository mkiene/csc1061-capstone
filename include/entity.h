#pragma once

#include <memory>
#include <vector>

#include "level.h"
#include "utils.h"

class Entity
{
   private:
    vec2 pos;
    char symbol;
    int depth;

   protected:
    std::shared_ptr<Level> level;

   public:
    Entity(vec2 pos = {0.0f, 0.0f}, char symbol = '?', int depth = 0, std::shared_ptr<Level> level = nullptr) : pos(pos), symbol(symbol), depth(depth), level(level){};

   public:
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

    // Movement

    virtual void move(int direction);
};

class Player : public Entity
{
   public:
    Player(vec2 pos = {0.0f, 0.0f}, char symbol = '@', int depth = 0, std::shared_ptr<Level> level = nullptr) : Entity(pos, symbol, depth, level){};

    void set_level(std::shared_ptr<Level> new_level)
    {
        level = new_level;
    }
};

extern std::vector<std::shared_ptr<Entity>> entities;
