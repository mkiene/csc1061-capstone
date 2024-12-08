/*
 * Max Kiene
 * Sunday, December 8th, 2024
 * CSC1061 Capstone
 */

#include "../include/terminal.h"

#include <iostream>

#ifdef _WIN32
HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
DWORD orig_mode = 0;
#else
struct termios orig_termios;
#endif

Terminal term;

/*
 * Enables raw mode for the terminal.
 * This allows the terminal to accept input without requiring the Enter key.
 */

void Terminal::enable_raw()
{
#ifdef _WIN32
    GetConsoleMode(hStdin, &orig_mode);
    DWORD new_mode = orig_mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
    SetConsoleMode(hStdin, new_mode);
#else
    tcgetattr(STDIN_FILENO, &orig_termios);  // Store initial terminal parameters
    atexit(disable_raw);                     // Define the exit method to restore settings

    struct termios raw = orig_termios;                       // Initialize struct for raw input
    raw.c_lflag &= static_cast<tcflag_t>(~(ECHO | ICANON));  // Disable echoing and canonical mode
    raw.c_cc[VMIN] = 0;                                      // Set minimum characters for noncanonical read
    raw.c_cc[VTIME] = 0;                                     // Set timeout for noncanonical read

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);  // Apply new terminal settings
#endif
}

/*
 * Disables raw mode for the terminal.
 * Restores the terminal to its original state.
 */

void Terminal::disable_raw()
{
#ifdef _WIN32
    SetConsoleMode(hStdin, orig_mode);
#else
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);  // Restore original terminal settings
#endif
}

/*
 * Updates the terminal's size by determining its current dimensions.
 * Stores the width and height of the terminal window.
 */

void Terminal::update_size()
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    size.x = static_cast<float>(csbi.srWindow.Right - csbi.srWindow.Left + 1);
    size.y = static_cast<float>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    size.x = static_cast<float>(w.ws_col);
    size.y = static_cast<float>(w.ws_row);
#endif
}

/*
 * Initializes the terminal by configuring its settings.
 * Disables stream synchronization, hides the cursor, clears the screen, and sets raw mode.
 */

void Terminal::init()
{
    std::ios::sync_with_stdio(false);  // Disable synchronization of C++ streams with C streams
    std::cout << "\033[?25l";          // Hide the cursor
    std::cout << "\033[2J\033[H";      // Clear the screen

    enable_raw();   // Enable raw mode
    update_size();  // Get the terminal size

    atexit(clean);  // Set cleanup function to execute on exit
}

/*
 * Cleans up terminal settings before exiting.
 * Disables raw mode, clears the screen, and shows the cursor.
 */

void Terminal::clean()
{
    disable_raw();                 // Restore original terminal settings
    std::cout << "\033[2J\033[H";  // Clear the screen
    std::cout << "\033[?25h";      // Show the cursor
}
