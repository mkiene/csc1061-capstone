/*
 * Max Kiene
 * Sunday, December 8th, 2024
 * CSC1061 Capstone
 */

#include "../include/utils.h"

#include <iostream>

#include "../include/level.h"

/*
 * Determines whether a straight line between two points is blocked by a specified character.
 * Uses Bresenham's line algorithm to trace the path between the start and end points.
 * Returns true if the line of sight is clear or the endpoint is reached without encountering a blocker.
 */

bool in_line_of_sight(vec2 start, vec2 end, char blocker)
{
    int x1 = static_cast<int>(start.x);
    int y1 = static_cast<int>(start.y);

    int x2 = static_cast<int>(end.x);
    int y2 = static_cast<int>(end.y);

    int dx = std::abs(x2 - x1);
    int dy = -std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx + dy, e2;

    while (true)
    {
        if (x1 == x2 && y1 == y2)  // Reached the end point
        {
            break;
        }

        e2 = 2 * err;
        if (e2 >= dy)  // Move horizontally
        {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx)  // Move vertically
        {
            err += dx;
            y1 += sy;
        }

        // Check if the current tile contains the blocking character
        if (levels[0]->get_tile(x1, y1).symbol == blocker)
        {
            if (x1 == x2 && y1 == y2)  // Allow reaching the endpoint if it is the blocker
                return true;

            return false;  // Blocker encountered before reaching the endpoint
        }
    }
    return true;
}

/*
 * Calculates the squared distance (Euclidean) between two points (x1, y1) and (x2, y2).
 * Avoids using square roots for better performance in comparisons.
 * Returns the squared distance as a float.
 */

float sqr_dist(float x1, float y1, float x2, float y2)
{
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

/*
 * Clears the terminal screen and resets the cursor position to the top-left corner.
 * Achieved using ANSI escape codes.
 */

void clear_screen()
{
    std::cout << "\033[2J\033[1;1H";
}
