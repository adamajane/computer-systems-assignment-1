# Computer Systems - Assignment 1

First assignment in the Computer Systems (02132) course.

## What does the program do?

The program is a simple cell detection program. It takes as an input an image of some cells and outputs an image with a detection marker on each cell.

## How to compile and run the program

### On macOS and Linux

Compiling the program: ```gcc cbmp.c main.c -o main.out -std=c99```

Running the program: ```./main.out example.bmp example_inv.bmp```

### On Windows

Compiling the program: ```gcc cbmp.c main.c -o main.exe -std=c99```

Running the program: ```main.exe example.bmp example_inv.bmp```
