#pragma once

#include <memory>
#include <vector>

#include "level.h"
#include "utils.h"

class Entity  // Abstract class for entities
{
   public:
    vec2 pos;
    char symbol;
    int depth;

    std::shared_ptr<Level> level;

    Entity(vec2 pos = {0.0f, 0.0f}, char symbol = '?', int depth = 0, std::shared_ptr<Level> level = nullptr) : pos(pos), symbol(symbol), depth(depth), level(level){};

    void init();
    void update();

    void move(int direction);

    virtual ~Entity() = default;
};

class Player : public Entity
{
   public:
    Player(vec2 pos = {0.0f, 0.0f}, char symbol = '@', int depth = 0, std::shared_ptr<Level> level = nullptr) : Entity(pos, symbol, depth, level){};

    void move(int direction);
};

extern std::vector<std::shared_ptr<Entity>> entities;
