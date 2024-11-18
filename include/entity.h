#pragma once

#include <memory>
#include <vector>

#include "level.h"
#include "utils.h"

class Entity  // Entity object. This is fairly straightforward. Player derives from this class
{
   public:
    vec2 pos;
    char symbol;  // What the viewport should render for the entity
    int depth;

    std::shared_ptr<Level> level;  // Which level the entity exists within

    Entity(vec2 pos = {0.0f, 0.0f}, char symbol = '?', int depth = 0, std::shared_ptr<Level> level = nullptr) : pos(pos), symbol(symbol), depth(depth), level(level){};

    void init();
    void update();

    void move(int direction);  // Move in a certain direction, checking for collisions

    virtual ~Entity() = default;
};

class Player : public Entity
{
   public:
    Player(vec2 pos = {0.0f, 0.0f}, char symbol = '@', int depth = 0, std::shared_ptr<Level> level = nullptr) : Entity(pos, symbol, depth, level){};

    void move(int direction);
};

extern std::vector<std::shared_ptr<Entity>> entities;  // Global list of entities
