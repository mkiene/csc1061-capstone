#ifdef _WIN32
#include <windows.h>  // windows api
#else
#include <sys/ioctl.h>  // system call for unix i/o operations
#include <termios.h>    // definitions used by unix terminal i/o interfaces
#include <unistd.h>     // POSIX API access
#endif

#include <math.h>  // math

#include <algorithm>  // various functions
#include <chrono>     // timekeeping
#include <iostream>   // standard i/o
#include <random>     // rng
#include <sstream>    // stringstreams
#include <thread>     // threading
#include <vector>     // vectors (dynamic arrays)

#ifdef _WIN32
HANDLE hStdin;    // handle for windows input stream
DWORD orig_mode;  // stores the original term params
#else
struct termios orig_termios;  // struct to store term params
#endif

const char wall_symb = '#';
const char empty_symb = ' ';
const char player_symb = '@';
const char floor_symb = '.';

int view_radius = 999;  // visible radius from player
const int torch_radius = 25;
int current_torch_radius = torch_radius;
int torch_flicker_amount = 0;
int torch_flicker_rate = 10;              // per second
bool clamp_viewport_to_level_bounds = 1;  // should the viewport be constrained to within the level
bool viewport_follow = 1;                 // should the viewport follow player movement
int viewport_slack_x = -1;                //-1 for auto, 0 for smooth, 1 for none
int viewport_slack_y = -1;                // -1 for auto, 0 for smooth, 1 for none
int term_width, term_height;              // terminal column and row dimensions
int x_scaling = 2;                        // stretches output horizontally
float viewport_follow_speed = 0.1f;       // if using lerp, how fast should the viewport follow.
int level_width = 300;                    // How wide the level should be
int level_height = 300;                   // How tall the level should be

bool rgb_mode = 1;  // Enable for terminal emulators that allow RGB representation, disable for standard terminals. I use iTerm2.

float max_color_mult_for_dimming = 0.86f;  // In RGB mode, how much should the background be dimmed when not visible.

class entity  // Abstract class for entities
{
   public:
    virtual void move(int dir) = 0;
    virtual ~entity()
    {
    }
};

struct ui_content  // For drawing text to the console at an x and y position.
{
    int x;
    int y;
    std::string content;
};

std::vector<ui_content> ui_draw_queue;  // For UI content

struct
{
    float x;  // Viewport location in the level. This is relative to level.x, level.y.
    float y;
    int term_x;  // Viewport location in the console
    int term_y;
    int width = 150 / x_scaling;  // dimensions of viewport
    int height = 45;
    std::vector<std::vector<char>> chars;  // 2-dimensional array of chars that will be drawn
    std::vector<std::vector<char>> prev_chars;
} viewport;

struct
{
    int x;
    int y;
    int width = level_width;
    int height = level_height;
    std::vector<std::vector<char>> chars;  // 2-dimensional array of chars representing level tiles
} level;

class player : public entity
{
   public:
    int x, y;

    player(int init_x, int init_y) : x(init_x), y(init_y)
    {
    }

    void move(int dir) override
    {
        switch (dir)
        {
            case 0:
                if (level.chars[y][x - 1] != wall_symb)
                    x--;
                break;
            case 1:
                if (level.chars[y - 1][x] != wall_symb)
                    y--;
                break;
            case 2:
                if (level.chars[y][x + 1] != wall_symb)
                    x++;
                break;
            case 3:
                if (level.chars[y + 1][x] != wall_symb)
                    y++;
                break;
        }
    }
};

struct rgb
{
    int r = 255;
    int g = 255;
    int b = 255;
} color;

rgb default_color = {255, 255, 255};
rgb player_color = {255, 255, 255};
rgb wall_color = {240, 240, 240};
rgb floor_color = {220, 220, 220};

player pl(1, 1);

std::vector<std::vector<rgb>> color_map;

std::string get_color(rgb color)  // returns an ANSI escape sequence for drawing a color
{
    return "\033[38;2;" + std::to_string(color.r) + ";" + std::to_string(color.g) + ";" + std::to_string(color.b) + "m";
}

void get_terminal_size(int &width, int &height)  // Had to look up how to do this
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    width = w.ws_col;
    height = w.ws_row;
#endif
}

int get_random_int(int min, int max)
{
    std::random_device dev;                                                   // for nondeterministic generation
    std::mt19937 rng(dev());                                                  // mersenne twister
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);  // equal probability
    return dist(rng);                                                         // result
}

void disable_raw()  // disable previously enabled raw mode, restoring from saved params
{
#ifdef _WIN32
    SetConsoleMode(hStdin, orig_mode);
#else
    tcsetattr(STDIN_FILENO, TCSAFLUSH,
              &orig_termios);  // return to initial term params
#endif
}

void enable_raw()  // Had to look this up as well. Allows the terminal to accept input without pressing "enter"
{
#ifdef _WIN32
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hStdin, &orig_mode);
    DWORD new_mode = orig_mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
    SetConsoleMode(hStdin, new_mode);
#else
    tcgetattr(STDIN_FILENO, &orig_termios);  // store initial term params
    atexit(disable_raw);                     // define exit method

    struct termios raw = orig_termios;  // init struct for raw input
    raw.c_lflag &= ~(ECHO | ICANON);    // disable echoing and canonical mode
    raw.c_cc[VMIN] = 0;                 // minimum number of characters for noncanonical read
    raw.c_cc[VTIME] = 0;                // timeout in deciseconds for noncanonical read

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);  // apply new term params
#endif
}

char get_ch()  // Had to look this up as well. Returns which key is pressed.
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

void draw_room(int x, int y, int w, int h, bool centered)  // Draws a rectangle to the level.chars 2d vector. This is currently used for demoing the lighting system.
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            int pos_x = x - ((centered) ? w / 2 : 0) + j;
            int pos_y = y - ((centered) ? h / 2 : 0) + i;

            if (pos_x > level.x && pos_x < level.x + level.width && pos_y > level.y && pos_y < level.y + level.height)
            {
                if (i == 0 || i == h - 1 || j == 0 || j == w - 1)
                    level.chars[level.y + pos_y][level.x + pos_x] = wall_symb;
                else
                    level.chars[level.y + pos_y][level.x + pos_x] = empty_symb;
            }
        }
    }
}

void init_level()  // Creates the array for the level
{
    for (auto y = 0; y < level.height; y++)
    {
        for (auto x = 0; x < level.width; x++)
        {
            level.chars[y][x] = (x == 0 || y == 0 || x == level.width - 1 || y == level.height - 1) ? wall_symb : floor_symb;  // init level, add walls at edges, add floor tiles
        }
    }

    for (int i = 0; i < level.width / 5; i++)
    {
        int w = get_random_int(5, 30);
        int h = get_random_int(5, 30);
        int x = get_random_int(0, level.width - w);
        int y = get_random_int(0, level.height - h);

        draw_room(x, y, w, h, 0);
    }
}

bool in_line_of_sight(int x1, int y1, int x2, int y2, char blocker)  // adaptation of bresenham's line algorithm
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

        // check the next tile after movement for obstacles
        if (level.chars[y1][x1] == blocker)
        {
            if (x1 == x2 && y1 == y2)
                return true;  // Line of sight includes the obstacle if it's the target

            return false;  // Line of sight is blocked by an obstacle
        }
    }
    return true;  // Line of sight is clear
}

int sqr_dist(int x1, int y1, int x2, int y2)  // Return squared distance, as sqrt is expensive and unnescessary for this program
{
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

rgb normalize_color(float sqr_dist, float max_radius, rgb color)  // Returns an adjusted RGB color based on distance from player
{
    rgb new_color = color;
    // Compute the distance ratio squared (squared to ensure non-linear falloff)
    float distance_ratio = sqr_dist / (max_radius * max_radius);

    // Clamp the ratio between 0 and 1
    distance_ratio = std::clamp(distance_ratio, 0.0f, max_color_mult_for_dimming);

    // Calculate the scaling factor: inverse of distance ratio
    float scale = 1.0f - distance_ratio;

    // Apply the scale to the original color
    new_color.r = std::max(0.0f, std::min(255.0f, color.r * scale));
    new_color.g = std::max(0.0f, std::min(255.0f, color.g * scale));
    new_color.b = std::max(0.0f, std::min(255.0f, color.b * scale));

    return new_color;
}

void draw_level()  // Draws relevant level chars to the viewport char array
{
    for (int y = 0; y < viewport.height; y++)
    {
        for (int x = 0; x < viewport.width; x++)
        {
            int real_x = (int)viewport.x - level.x + x;
            int real_y = (int)viewport.y - level.y + y;

            bool within_viewport = (real_y >= 0 && real_y < level.height && real_x >= 0 && real_x < level.width);

            if (within_viewport)
            {
                if (real_x == pl.x && real_y == pl.y)
                {
                    viewport.chars[y][x] = player_symb;
                }
                else
                {
                    bool within_range = sqr_dist(pl.x, pl.y, real_x, real_y) <= view_radius * view_radius;

                    if (within_range)
                    {
                        if (rgb_mode)
                        {
                            viewport.chars[y][x] = level.chars[real_y][real_x];
                        }
                        else if (sqr_dist(pl.x, pl.y, real_x, real_y) <= current_torch_radius * current_torch_radius && in_line_of_sight(pl.x, pl.y, real_x, real_y, wall_symb))
                        {
                            viewport.chars[y][x] = level.chars[real_y][real_x];
                        }
                        else
                        {
                            viewport.chars[y][x] = ' ';
                        }
                    }
                }
            }
        }
    }
}

void init()  // Initializes various things. Needs to be cleaned
{
    std::ios::sync_with_stdio(false);  // disable stream sync
    std::cout << "\033[?25l";          // hide cursor

    enable_raw();  // enable raw mode

    get_terminal_size(term_width, term_height);

    level.chars.resize(level.height);

    for (auto &row : level.chars)
    {
        row.assign(level.width, empty_symb);
    }

    level.x = 0;  // center level on screen
    level.y = 0;

    viewport.width = std::min((term_width / x_scaling) - 10,
                              viewport.width);  // calculate viewport dimensions,
                                                // factoring in terminal dimensions
    viewport.height = std::min(term_height - 10, viewport.height);

    viewport.chars.resize(viewport.height);  // init viewport

    for (auto &row : viewport.chars)
    {
        row.assign(viewport.width, empty_symb);
    }

    viewport.prev_chars = viewport.chars;

    viewport.term_x = (term_width / (2 * x_scaling)) - (viewport.width / 2);  // center viewport on screen
    viewport.term_y = (term_height / 2) - (viewport.height / 2);
    viewport.x = pl.x - (int)(viewport.width / 2);
    viewport.y = pl.y - (int)(viewport.height / 2);

    color_map.resize(level.height);

    for (auto &row : color_map)
    {
        row.assign(level.width, default_color);
    }

    if (viewport_slack_x == -1)
        viewport_slack_x = viewport.width / 4;

    if (viewport_slack_y == -1)
        viewport_slack_y = viewport.height / 4;

    init_level();  // initialize level tiles
    draw_level();  // draw level tiles to viewport
}

void clean()  // Exit function
{
    disable_raw();
    std::cout << "\033[2J\033[H";  // clear screen
    std::cout << "\033[?25h";      // show cursor
}

void input(char &inp)  // Manage input
{
    inp = get_ch();

    switch (inp)
    {
        case 'h':  // move player left
            pl.move(0);
            break;
        case 'k':  // move player up
            pl.move(1);
            break;
        case 'l':  // move player right
            pl.move(2);
            break;
        case 'j':  // move player down
            pl.move(3);
            break;
    }
}

void move_viewport()  // Move the viewport. Can smoothy follow, exactly follow, or allow the player to move a certain distance in either axis before following. NOTE: std::lerp requires cpp 20, so don't compile with 17.
{
    auto target = pl;

    int viewport_center_x = (int)viewport.x + (viewport.width / 2);
    int viewport_center_y = (int)viewport.y + (viewport.height / 2);

    if (viewport_follow)
    {
        if (viewport_slack_x + viewport_slack_y > 0)
        {
            if (std::abs(viewport_center_x - target.x) > viewport_slack_x / 2)
            {
                viewport.x += (viewport_center_x - target.x < 0) ? 1 : -1;
            }

            if (std::abs(viewport_center_y - target.y) > viewport_slack_y / 2)
            {
                viewport.y += (viewport_center_y - target.y < 0) ? 1 : -1;
            }
        }
        else
        {
            viewport.x = std::lerp(viewport.x, level.x - (viewport.width / 2) + target.x, viewport_follow_speed);
            viewport.y = std::lerp(viewport.y, level.y - (viewport.height / 2) + target.y, viewport_follow_speed);
        }
    }

    if (level.width >= viewport.width && clamp_viewport_to_level_bounds)
    {
        float xmin = level.x;
        float xmax = level.x + level.width - viewport.width;
        viewport.x = std::clamp(viewport.x, xmin, xmax);
    }

    if (level.height >= viewport.height && clamp_viewport_to_level_bounds)
    {
        float ymin = level.y;
        float ymax = level.y + level.height - viewport.height;
        viewport.y = std::clamp(viewport.y, ymin, ymax);
    }
}

template <typename... Args>
void draw_ui_content(int x, int y, Args &&...args)  // Allows n arguments, and different datatypes!
{
    std::stringstream ss;

    // Fold expression to unpack and insert all arguments into the stringstream
    (ss << ... << args);

    // Retrieve the concatenated string
    std::string formatted_content = ss.str();

    // Create a ui_content struct and push it to the drawing queue
    ui_content item = {x, y, formatted_content};
    ui_draw_queue.push_back(item);
}

void draw_ui(std::stringstream &ui_stream)  // Draw ui content at x and y.
{
    draw_ui_content(1, 1, "player x: ", pl.x, " ");  // example output
    draw_ui_content(1, 2, "player y: ", pl.y, " ");

    for (const auto &i : ui_draw_queue)
    {
        ui_stream << "\033[" << i.y << ";" << i.x << "H" << i.content;
    }
    ui_draw_queue.clear();  // Clear the queue after processing all elements
}

void draw()  // Draws chars from viewport buffer to the console using ANSI esc sequences
{
    std::stringstream ss;  // create a stringstream to buffer all output

    draw_level();  // populate the level data into the viewport

    draw_ui(ss);  // pass buffer stringstream to the ui function

    // loop through the viewport and buffer the draw commands into the stringstream
    for (int y = 0; y < viewport.height; y++)
    {
        for (int x = 0; x < viewport.width; x++)
        {
            int real_x = viewport.term_x + x;
            int real_y = viewport.term_y + y;

            switch (viewport.chars[y][x])
            {
                case player_symb:
                    color_map[viewport.y + y][viewport.x + x] = player_color;
                    break;
                case wall_symb:
                    color_map[viewport.y + y][viewport.x + x] = wall_color;
                    break;
                case floor_symb:
                    color_map[viewport.y + y][viewport.x + x] = floor_color;
                    break;
            }

            rgb draw_color;

            if (in_line_of_sight(pl.x, pl.y, viewport.x + x, viewport.y + y, wall_symb))
            {
                draw_color = normalize_color(sqr_dist(pl.x, pl.y, viewport.x + x, viewport.y + y), current_torch_radius, color_map[(int)viewport.y + y][(int)viewport.x + x]);
            }
            else
            {
                int r = color_map[viewport.y + y][viewport.x + x].r - (color_map[viewport.y + y][viewport.x + x].r * max_color_mult_for_dimming);
                int g = color_map[viewport.y + y][viewport.x + x].g - (color_map[viewport.y + y][viewport.x + x].g * max_color_mult_for_dimming);
                int b = color_map[viewport.y + y][viewport.x + x].b - (color_map[viewport.y + y][viewport.x + x].b * max_color_mult_for_dimming);

                draw_color = {r, g, b};
            }

            ss << "\033[" << real_y << ";" << real_x * x_scaling << "H" << ((rgb_mode) ? get_color(draw_color) : "") << viewport.chars[y][x] << "\033[0m";
            viewport.prev_chars[y][x] = viewport.chars[y][x];
        }
    }

    // flush the entire buffer to standard output at once; this reduces flickering.
    std::cout << ss.str();
    std::cout.flush();  // ensure the output is sent to the terminal immediately
}

int main()  // main loop, using target 120fps. Implemented via chrono!
{
    init();

    const int FPS = 120;                                         // Target Frames Per Second
    const std::chrono::milliseconds frame_duration(1000 / FPS);  // Frame duration in milliseconds
    auto last_frame_time = std::chrono::steady_clock::now();

    char inp = 0;  // Global input handler

    int counter = 0;

    while (inp != 27)
    {
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_frame_time);

        if (elapsed_time >= frame_duration)
        {
            move_viewport();
            draw();
            input(inp);

            if (rgb_mode)
            {
                if (counter >= FPS / torch_flicker_rate)
                {
                    current_torch_radius = get_random_int(torch_radius - torch_flicker_amount, torch_radius + torch_flicker_amount);
                    counter = 0;
                }
                else
                {
                    counter++;
                }
            }

            last_frame_time = current_time;

            // Calculate how much time to sleep to maintain the constant FPS
            auto frame_end_time = std::chrono::steady_clock::now();
            auto frame_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end_time - last_frame_time);
            if (frame_duration > frame_elapsed)
            {
                std::this_thread::sleep_for(frame_duration - frame_elapsed);
            }
        }
    }

    clean();

    return 0;
}
