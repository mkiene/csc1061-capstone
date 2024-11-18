#include "../include/config.h"
#include "../include/entity.h"
#include "../include/level.h"
#include "../include/terminal.h"
#include "../include/viewport.h"

// Initialize the game. This sets up the terminal, level, viewport, and player.
void init_game()
{
    term.init();  // Initialize the terminal for the game. File: terminal.h/cpp

    auto level = std::make_shared<Level>();  // Create a level object. File: level.h/cpp
    level->size = {100, 100};                // Set the level size to be 100x100. File: level.h/cpp
    level->depth = 0;                        // Set the level drawing  depth. File: level.h/cpp
    level->init();                           // Initialize the level. File: level.h/cpp
    levels.push_back(level);                 // Add the level to a global list of levels. File: level.h/cpp

    auto viewport = std::make_shared<Viewport>();  // Create a viewport object. File: viewport.h/cpp
    viewport->size = {100, 40};                    // Set the viewport size to 100x40. File: viewport.h/cpp
    viewport->level = level;                       // Set the viewport's level (which it will display). File: viewport.h/cpp
    viewport->init();                              // Initialize the viewport. File: viewport.h/cpp
    viewports.push_back(viewport);                 // Add the viewport to a global list of viewports. File: viewport.h/cpp

    auto player = std::make_shared<Player>();  // Create a Player object (inherits from Entity). File: Entity.h/cpp
    player->pos = {5.0f, 5.0f};                // Set the player position to (5,5). This is relative to the origin of the level. File: entity.h/cpp
    player->depth = -10;                       // Set the player drawing depth. File: entity.h/cpp
    player->init();                            // Initialize the player. File: entity.h/cpp
    player->level = level;                     // Set the player's level. File: entity.h/cpp
    entities.push_back(player);                // Add the player to a global list of entities. File: entity.h/cpp
}

// Update the game state. This currently only updates the viewport list.

void update_game()
{
    for (const auto& viewport : viewports)  // Loop through the viewport list. File: viewport.h/cpp
    {
        viewport->draw();              // Draw the contents of the level to the viewport. File: viewport.h/cpp
        viewport->move(*entities[0]);  // Update the position of the viewport to follow the player (entity) object. File: viewport.h/cpp
    }
}
