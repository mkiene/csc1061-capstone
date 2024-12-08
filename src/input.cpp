/*
 * Max Kiene
 * Sunday, December 8th, 2024
 * CSC1061 Capstone
 */

#include "../include/input.h"

#include "../include/config.h"
#include "../include/entity.h"
#include "../include/game.h"
#include "../include/generation.h"
#include "../include/terminal.h"
#include "../include/ui.h"
#include "../include/utils.h"
#include "../include/viewport.h"

char inp = 0;

// get_ch(): Get input

char get_ch()
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

// Called when the game is unpaused

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

            // Player movement
        case 'h':  // Move player left
        case 'a':
            player->move(0);
            break;
        case 'k':  // Move player up
        case 'w':
            player->move(1);
            break;
        case 'l':  // Move player right
        case 'd':
            player->move(2);
            break;
        case 'j':  // Move player down
        case 's':
            player->move(3);
            break;

        case 'n':  // Restart demo
            init_game();
            break;

        case 't':  // Toggle LOS
            enable_LOS = !enable_LOS;
            break;

            // Refresh viewport
        case 'r':
            viewports[0]->init();
            break;
    }
}

// Called when the game is paused

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
        case 51:  // 3: Quit
            menu.exit_game();
            break;
    }
}

// Directs input

void input(char& inp)
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
