#include <ranges>

#include "../include/config.h"
#include "../include/entity.h"
#include "../include/level.h"
#include "../include/terminal.h"
#include "../include/viewport.h"

void init_game()
{
    term.init();

    auto level = std::make_shared<Level>();
    level->size = {100, 100};
    level->depth = 0;
    level->init();
    levels.push_back(level);

    auto viewport = std::make_shared<Viewport>();
    viewport->size = {100, 40};
    viewport->level = level;
    viewport->init();
    viewports.push_back(viewport);

    auto player = std::make_shared<Player>();
    player->pos = {5.0f, 5.0f};
    player->depth = -10;
    player->init();
    player->level = level;
    entities.push_back(player);
}

void update_game()
{
    for (const auto& viewport : viewports)
    {
        viewport->draw();
        viewport->move(*entities[0]);
    }
}
