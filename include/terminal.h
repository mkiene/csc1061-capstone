#pragma once

#include "utils.h"

#ifdef _WIN32         // windows-specific setup
#define NOMINMAX      // remove the macros for min and max
#include <windows.h>  // windows api
#undef NOMINMAX
extern HANDLE hStdin;    // handle for windows input stream
extern DWORD orig_mode;  // stores the original term params
#else
#include <sys/ioctl.h>  // system call for unix i/o operations
#include <termios.h>    // definitions used by unix terminal i/o interfaces
#include <unistd.h>     // POSIX API access
extern struct termios orig_termios;  // stores the original term params
#endif

// Terminal object. There should only be one, which manages the terminal window.
class Terminal
{
   public:
    vec2 size;  // The size of the terminal window. This is determined once at initialization.

    void init();          // Initialize the terminal window.
    static void clean();  // Return the terminal window to it's previous state.

   private:
    void enable_raw();          // Enables the terminal to accept non-blocking input.
    static void disable_raw();  // Disables above.
    void update_size();         // Updates the 'vec2 size' property.
};
