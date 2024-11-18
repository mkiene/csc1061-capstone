#include "../include/game.h"
#include "../include/input.h"

int main()
{
    init_game();

    char inp = 0;  // Global input handler

    while (inp != 'Q')
    {
        update_game();
        input(inp);
    }

    return 0;
}
