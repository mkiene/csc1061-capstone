#include "../include/utils.h"

#include <random>

#include "../include/config.h"

int get_random_int(int min, int max)
{
    std::random_device dev;                                                   // for nondeterministic generation
    std::mt19937 rng(dev());                                                  // mersenne twister
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);  // equal probability
    return dist(rng);                                                         // result
}

bool in_line_of_sight(int x1, int y1, int x2, int y2, char blocker)  // adaptation of bresenham's line algorithm.
{
    int dx = std::abs(x2 - x1);
    int dy = -std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx + dy, e2;  // error value e_xy

    while (true)
    {
        if (x1 == x2 && y1 == y2)
        {
            break;  // Line has reached the target
        }

        e2 = current_torch_radius * 2 * err;
        if (e2 >= dy)
        {  // e_xy + e_x > 0
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx)
        {  // e_xy + e_y < 0
            err += dx;
            y1 += sy;
        }

        /*

        // check the next tile after movement for obstacles
        if (level.chars[y1][x1] == blocker)
        {
            if (x1 == x2 && y1 == y2)
                return true;  // Line of sight includes the obstacle if it's the target

            return false;  // Line of sight is blocked by an obstacle
        }

        */
    }
    return true;  // Line of sight is clear
}

int sqr_dist(int x1, int y1, int x2, int y2)  // Return squared distance, as sqrt is expensive and unnescessary for this program
{
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}
