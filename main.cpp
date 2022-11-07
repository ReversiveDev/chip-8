// Using SDL and standard IO
// #include <SDL2/SDL.h>
#include <stdio.h>
#include "chip8.hpp"

// Screen dimension constants
// const int SCREEN_WIDTH = 640;
// const int SCREEN_HEIGHT = 480;

int main(int argc, char *argv[])
{
    Chip8 chip8;

    if(argc < 2) {
        printf("Especifique o nome do arquivo ROM.\n");
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        perror("fopen");
        return 1;
    }

    chip8.LoadRom(file);

    return 0;
}