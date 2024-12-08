/*
 * Max Kiene
 * Sunday, December 8th, 2024
 * CSC1061 Capstone
 */

#include "../include/game.h"
#include "../include/input.h"

int main()
{
    init_game();

    while (inp != 'Q')
    {
        update_game();
        input(inp);
    }

    return 0;
}
