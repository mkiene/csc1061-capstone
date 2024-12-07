#include "../include/terminal.h"

#include <iostream>

#ifdef _WIN32
HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
DWORD orig_mode = 0;
#else
struct termios orig_termios;
#endif

Terminal term;

void Terminal::enable_raw()  // Allows the terminal to accept input without pressing "enter"
{
#ifdef _WIN32
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

void Terminal::disable_raw()  // disable previously enabled raw mode, restoring from saved params
{
#ifdef _WIN32
    SetConsoleMode(hStdin, orig_mode);
#else
    tcsetattr(STDIN_FILENO, TCSAFLUSH,
              &orig_termios);  // return to initial term params
#endif
}

void Terminal::update_size()  // Determine the dimensions of the terminal window
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    size.x = (float)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
    size.y = (float)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    size.x = (float)w.ws_col;
    size.y = (float)w.ws_row;
#endif
}

void Terminal::init()
{
    std::ios::sync_with_stdio(false);  // disable stream sync
    std::cout << "\033[?25l";          // hide the cursor
    std::cout << "\033[2J\033[H";      // clear screen

    enable_raw();

    update_size();

    atexit(clean);
}

void Terminal::clean()  // Exit function
{
    disable_raw();
    std::cout << "\033[2J\033[H";  // clear screen
    std::cout << "\033[?25h";      // show cursor
}
