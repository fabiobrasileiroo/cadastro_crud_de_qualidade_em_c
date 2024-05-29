#!/bin/bash

make

./src/order_management

make clean

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Executable created: program"
else
    echo "Compilation failed."
fi
