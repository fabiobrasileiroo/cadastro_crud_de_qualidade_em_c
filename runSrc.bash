#!/bin/bash

# Compile the C program with GTK+ 3.0
gcc -o src/order_management src/main.c src/orders.c src/display_orders.c src/login.c `pkg-config --cflags --libs gtk+-3.0`
./src/order_management

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Executable created: program"
else
    echo "Compilation failed."
fi
