#include "../include/input.h"

#include <iostream>
#include <ranges>

#include "../include/config.h"
#include "../include/entity.h"
#include "../include/game.h"
#include "../include/generation.h"
#include "../include/terminal.h"
#include "../include/ui.h"
#include "../include/utils.h"
#include "../include/viewport.h"

char get_ch()  // Retrieve keyboard input
{
#ifdef _WIN32
    char c = 0;
    DWORD read;
    ReadConsole(hStdin, &c, 1, &read, NULL);
    return c;
#else
    char c;
    if (read(STDIN_FILENO, &c, 1) != 1)
        c = '\0';
    return c;
#endif
}

void handle_gameplay_input(char& inp)
{
    Entity* player = entities[0].get();

    switch (inp)
    {
        case 27:
            clear_screen();
            menu.trigger_titlescreen();
            game_paused = true;
            break;
            // player movement
        case 'h':  // move player left
            player->move(0);
            break;
        case 'k':  // move player up
            player->move(1);
            break;
        case 'l':  // move player right
            player->move(2);
            break;
        case 'j':  // move player down
            player->move(3);
            break;

        case 'n':
            init_game();
            break;

        case 't':
            enable_LOS = !enable_LOS;
            break;

            // viewport movement

            if (viewports.size() > 0)
            {
                case 'w':  // move viewport[0] up
                    viewports[0]->set_level_pos({viewports[0]->get_level_pos().x, viewports[0]->get_level_pos().y - 1.0f});
                    break;
                case 'a':  // move viewport[0] left
                    viewports[0]->set_level_pos({viewports[0]->get_level_pos().x - 1.0f, viewports[0]->get_level_pos().y});
                    break;
                case 's':  // move viewport[0] down
                    viewports[0]->set_level_pos({viewports[0]->get_level_pos().x, viewports[0]->get_level_pos().y + 1.0f});
                    break;
                case 'd':  // move viewport[0] right
                    viewports[0]->set_level_pos({viewports[0]->get_level_pos().x + 1.0f, viewports[0]->get_level_pos().y});
                    break;

                    // refresh viewport
                case 'r':
                    viewports[0]->init();
                    break;
            }
    }
}

void handle_menu_input(char& inp)
{
    switch (inp)
    {
        case 27:  // ESC
            game_paused = false;
            clear_screen();
            break;
        case 49:  // 1: Start
            break;
        case 50:  // 2: Controls
            menu.trigger_controls();
            break;
        case 51:  // 3: README
            break;
    }
}

void input(char& inp)  // Direct input
{
    inp = get_ch();

    if (!game_paused)
    {
        handle_gameplay_input(inp);
    }
    else
    {
        handle_menu_input(inp);
    }
}
