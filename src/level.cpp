#include "../include/level.h"

#include <vector>

std::vector<std::shared_ptr<Level>> levels;  // Define the global level list

// Level object init function. When called, this updates the 'tiles' matrix to reflect the chosen level size when created, and assigns Level objects with randomized properties.
void Level::init()
{
    tiles.resize(static_cast<size_t>(size.y), std::vector<Tile>(static_cast<size_t>(size.x)));  // resize the matrix

    for (size_t y = 0; y < size.y; ++y)
    {
        for (size_t x = 0; x < size.x; ++x)
        {
            int decision = get_random_int(0, 50);  // choose a random number between 0 and 50

            if (decision == 0)
            {
                tiles[y][x] = {depth - 10, '#'};  // If chosen number is zero, create a blocking object with the depth -10
            }
            else
            {
                tiles[y][x] = {depth, '.'};  // Otherwise, the tile is a floor object.
            }
        }
    }
}
