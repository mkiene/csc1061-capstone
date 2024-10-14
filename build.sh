#!/bin/bash

# Get the directory of the script
script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"

# Check if user provided filename
if [ -z "$1" ]; then
  echo "Usage: $0 <cpp_file>"
  exit 1
fi

# Get absolute path of the cpp file
cpp_file="$(realpath "$1")"

# Verify that the cpp file exists
if [ ! -f "$cpp_file" ]; then
  echo "Error: File '$cpp_file' does not exist."
  exit 1
fi

# Get filename without extension
filename=$(basename "$cpp_file" .cpp)

# Define build directory based on script's directory
build_dir="$script_dir/build"

# Create build directory if it doesn't exist
mkdir -p "$build_dir"

# Clear terminal
clear

# List of common libs
common_libs=("sdl2" "sfml-graphics" "sfml-window" "sfml-system" "gtk+-3.0" "opencv4" "glfw3" "glew" "cairo" "ncurses") 

# Initialize libs string
libraries=""

# Auto-detect libs
for lib in "${common_libs[@]}"; do
  if pkg-config --exists "$lib"; then
    libraries+=$(pkg-config --cflags --libs "$lib")" "
  fi
done

# Remove duplicate libs
libraries=$(echo "$libraries" | tr ' ' '\n' | sort -u | tr '\n' ' ')

# Compile and place in build dir
g++ -std=c++20 -o "$build_dir/$filename" "$cpp_file" $libraries

# Check if compilation was successful
if [ $? -eq 0 ]; then
  # echo "Compilation successful. Executable is in $build_dir"
  # Run the executable
  "$build_dir/$filename"
else
  echo "Compilation failed."
  echo "Tried: g++ -std=c++20 -o \"$build_dir/$filename\" \"$cpp_file\""
  exit 1
fi
