# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(Torch LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the paths to your source and include directories
include_directories(include)
file(GLOB SOURCES src/*.cpp)

# Add the executable
add_executable(Torch ${SOURCES})
