/*
#   In order to interact with this program, you can use the h, j, k, and l keys
# to move the character '@' around. In order to quit the program, you must input
# 'Q' (uppercase). Other than that, the program has little interaction as of now.
#
#   This file, './src/main.cpp', is the entrypoint for the program. This is used to
# call general setup functions and to enter the main loop for the program. The file
# which handles most of the setup is the './src/game.cpp' file. From there, it
# should be fairly straighforward to follow the functionality of the program.
#
#   It is important to note that not all .cpp and .h files are currently used,
# although I do plan on implementing them for the final version of the project.
# They are remnants from the process of converting everything from a single .cpp
# file to incoporate headers and implementations (which I should have done in the
# first place).
#
# ------------------------------------IMPORTANT------------------------------------
#
#   To compile and run this project, you should use CMake. This project was built
# and tested using MacOS using CMake. You can download CMake at https://cmake.org.
#
#   To compile using CMake, follow these steps:
#
#   1. In a terminal window, navigate to the directory './build/', where '.' is the
#       root directory for the project.  This 'build' directory should be empty. If
#       this directory isn't empty, you should delete all the files within it.
#
#   2. Run 'cmake ..'. This will create the nescessary files for CMake to build the
#       project.
#
#   3. Run 'cmake --build .'. This will build the project and create an executable
#       file witin the 'build' directory. It may be located in './build/debug/', if
#       not in './build/'. This executable will be called 'Torch.exe' or similar.
#
#   4. In the directory of the executable file 'Torch', you can run './Torch' from
#       your terminal window, which should run the compiled program.
#
#   If these steps do not work, then please let me know - it has been difficult to
# maintain cross-compatability, given the nature of capturing keyboard input from
# system-specific terminals.
*/

#include "../include/game.h"
#include "../include/input.h"

int main()  // Program entrypoint
{
    init_game();  // Initialize various things. File: game.cpp

    char inp = 0;  // Tracks global input

    while (inp != 'Q')  // Start main loop
    {
        update_game();  // Update the game. File: game.cpp
        input(inp);     // Gather input. File: input.cpp
    }

    return 0;  // Program exit
}
