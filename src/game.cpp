#include <memory>

#include "../include/config.h"
#include "../include/entity.h"
#include "../include/generation.h"
#include "../include/level.h"
#include "../include/menu.h"
#include "../include/terminal.h"
#include "../include/ui.h"
#include "../include/utils.h"
#include "../include/viewport.h"

bool game_paused = true;

void init_game()
{
    levels.clear();
    entities.clear();
    viewports.clear();

    term.init();

    auto level = std::make_shared<Level>(vec2(0.0f, 0.0f), vec2(100.0f, 100.0f), 0);
    levels.push_back(level);

    auto player = std::make_shared<Player>();
    player->set_pos({20.0f, 20.0f});
    player->set_depth(-5);
    player->set_level(level);
    entities.push_back(player);

    auto viewport = std::make_shared<Viewport>();
    viewport->set_size({1000, 1000});
    viewport->set_level_pos({-10, -5});
    viewport->set_level(level);
    viewport->set_target(player);
    viewport->init();
    viewports.push_back(viewport);

    auto generator = std::make_shared<Generator>();
    generator->set_level(level);
    generator->rogue(7, vec2(10.0f, 10.0f), {30.0f, 30.0f}, 3);
    generator->automata(8, 35, 5, 4);
    generator->remove_inaccessible();
    generators.push_back(generator);

    if (game_paused)
        menu.trigger_titlescreen();
}

// Update the game state. This currently only updates the viewport list.

void update_game()
{
    for (const auto& viewport : viewports)
    {
        viewport->update();
    }

    if (game_paused)
    {
        draw_ui();
    }
}
