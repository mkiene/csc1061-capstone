#include "../include/room.h"

#include "../include/config.h"

void Room::create()
{
    for (int i = 0; i < size.y; i++)
    {
        for (int j = 0; j < size.x; j++)
        {
            int pos_x = pos.x + j;
            int pos_y = pos.y + i;

            if (pos_x > level.pos.x && pos_x < level.pos.x + level.size.x && pos_y > level.pos.y && pos_y < level.pos.y + level.size.y)
            {
                if (i == 0 || i == size.y - 1 || j == 0 || j == size.x - 1)
                    level.tiles[level.pos.y + pos_y][level.pos.x + pos_x].symbol = wall_symb;
                else
                    level.tiles[level.pos.y + pos_y][level.pos.x + pos_x].symbol = empty_symb;
            }
        }
    }
}