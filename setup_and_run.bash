#!/bin/bash

# Check if GTK+ 3.0 is installed
if ! dpkg -l | grep -q libgtk-3-dev; then
    echo "GTK+ 3.0 not found. Installing..."
    sudo apt-get update
    sudo apt-get install -y libgtk-3-dev
else
    echo "GTK+ 3.0 is already installed."
fi

# Compile the C program
gcc melhordeTodos.c -o program $(pkg-config --cflags --libs gtk+-3.0)

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Executable created: program"
    ./program
else
    echo "Compilation failed."
fi
