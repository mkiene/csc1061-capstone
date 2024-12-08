/*
 * Max Kiene
 * Sunday, December 8th, 2024
 * CSC1061 Capstone
 */

#include "../include/game.h"

#include "../include/entity.h"
#include "../include/generation.h"
#include "../include/level.h"
#include "../include/terminal.h"
#include "../include/ui.h"
#include "../include/utils.h"
#include "../include/viewport.h"

bool game_paused = true;

/*
 * init_game(): Intitalizes the game. This function is called directly from int main().
 * The objects created in this function must be created in a somewhat specific order because
 * of dependencies.
 */

void init_game()
{
    levels.clear();
    entities.clear();
    monsters.clear();
    viewports.clear();

    term.init();

    auto level = std::make_shared<Level>(vec2(0.0f, 0.0f), vec2(200.0f, 200.0f), 0);
    levels.push_back(level);

    auto player = std::make_shared<Player>();
    player->set_pos(vec2(20.0f, 20.0f));
    player->set_depth(-5);
    player->set_level(level);
    entities.push_back(player);

    auto viewport = std::make_shared<Viewport>();
    viewport->set_size(vec2(1000, 1000));
    viewport->set_level(level);
    viewport->set_level_pos(vec2(0.0f, 0.0f));
    viewport->set_target(player);
    viewport->init();
    viewports.push_back(viewport);

    auto generator = std::make_shared<Generator>();
    generator->set_level(level);
    generator->rogue(10, vec2(20.0f, 20.0f), vec2(40.0f, 40.0f), 5);
    generator->automata(8, 35, 5, 4);
    generator->remove_inaccessible();
    generators.push_back(generator);

    mass_spawn(level, 0.005f);

    if (game_paused)
        menu.trigger_titlescreen();
}

// Update the game's state

void update_game()
{
    for (const auto& viewport : viewports)
    {
        viewport->update();
    }

    for (const auto& monster : monsters)
    {
        monster->move(0);
    }

    draw_ui();
}
