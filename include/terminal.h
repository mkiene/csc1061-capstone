/*
 * Max Kiene
 * Sunday, December 8th, 2024
 * CSC1061 Capstone
 */

#pragma once

#include "utils.h"

#ifdef _WIN32
/*
 * Windows-specific setup.
 * Defines NOMINMAX to avoid conflicts with standard library min/max macros.
 * Includes <windows.h> for Windows API access.
 */
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

/*
 * Global variables for Windows terminal configuration.
 * hStdin: Handle for the standard input stream.
 * orig_mode: Stores the original terminal mode for future restoration.
 */
extern HANDLE hStdin;
extern DWORD orig_mode;

#else
/*
 * Unix-specific setup.
 * Includes necessary headers for terminal I/O operations:
 * - <sys/ioctl.h> for system calls related to I/O.
 * - <termios.h> for terminal I/O interface definitions.
 * - <unistd.h> for access to POSIX API.
 */
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

/*
 * Global variable for Unix terminal configuration.
 * orig_termios: Stores the original terminal settings for restoration.
 */
extern struct termios orig_termios;

#endif

/*
 * Terminal class.
 *
 * The Terminal class is responsible for managing the terminal window state
 * and configuration. It provides methods for initializing the terminal,
 * enabling and disabling raw input mode, updating the terminal's size,
 * and cleaning up by restoring the terminal to its original state.
 *
 * Public Members:
 * - size: Represents the terminal's width and height. This value
 *   is determined during initialization and can be updated dynamically.
 *
 * - init(): Prepares the terminal for use by configuring input
 *   settings and determining its size. This method must be called before
 *   using other Terminal functionalities.
 *
 * - clean(): Restores the terminal to its original settings,
 *   ensuring any temporary changes made during the program's execution
 *   are undone.
 *
 * Private Members:
 * - enable_raw(): Switches the terminal to raw input mode. In this mode,
 *   input is processed without waiting for a newline or interpreting special keys.
 *
 * - disable_raw(): Reverts the terminal from raw input mode to its
 *   standard configuration. This function is typically called during cleanup.
 *
 * - update_size(): Queries the terminal dimensions and updates the
 *   size member variable to reflect the current width and height.
 *
 * Usage:
 * Only a single instance of the Terminal class should exist throughout the
 * program's lifecycle. A global term instance is provided to manage the
 * terminal state centrally.
 */

class Terminal
{
   public:
    vec2 size;

    void init();

    static void clean();

   private:
    void enable_raw();

    static void disable_raw();

    void update_size();
};

extern Terminal term;
