#!/bin/bash

# Compile the C program with GTK+ 3.0
gcc melhordeTodos.c -o program $(pkg-config --cflags --libs gtk+-3.0)

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Executable created: program"
else
    echo "Compilation failed."
fi

./program