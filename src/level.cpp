#include "../include/level.h"

#include <vector>

std::vector<std::shared_ptr<Level>> levels;

void Level::init()
{
    tiles.resize(static_cast<size_t>(size.y), std::vector<Tile>(static_cast<size_t>(size.x)));

    for (size_t y = 0; y < size.y; ++y)
    {
        for (size_t x = 0; x < size.x; ++x)
        {
            int decision = get_random_int(0, 50);

            if (decision == 0)
            {
                tiles[y][x] = {depth - 10, '#'};
            }
            else
            {
                tiles[y][x] = {depth, '.'};
            }
        }
    }
}
