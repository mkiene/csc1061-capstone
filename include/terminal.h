#pragma once

#include "utils.h"

#ifdef _WIN32
#include <windows.h>  // windows api
HANDLE hStdin;        // handle for windows input stream
DWORD orig_mode;      // stores the original term params
#else
#include <sys/ioctl.h>  // system call for unix i/o operations
#include <termios.h>    // definitions used by unix terminal i/o interfaces
#include <unistd.h>     // POSIX API access
extern struct termios orig_termios;
#endif

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
