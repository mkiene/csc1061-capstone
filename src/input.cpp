#include "../include/input.h"

#include "../include/config.h"
#include "../include/entity.h"
#include "../include/terminal.h"
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

void input(char& inp)  // Direct input
{
    inp = get_ch();

    Entity* player = entities[0].get();

    switch (inp)
    {
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

            // viewport movement

        case 'w':  // move viewport[0] up
            viewports[0]->level_pos.y -= 1.0f;
            break;
        case 'a':  // move viewport[0] left
            viewports[0]->level_pos.x -= 1.0f;
            break;
        case 's':  // move viewport[0] down
            viewports[0]->level_pos.y += 1.0f;
            break;
        case 'd':  // move viewport[0] right
            viewports[0]->level_pos.x += 1.0f;
            break;
    }
}
