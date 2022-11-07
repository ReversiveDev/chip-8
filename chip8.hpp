#ifndef CHIP8_H
#define CHIP8_H
#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "vm.hpp"

class Chip8 : VM
{
public:
    Chip8();

    static int Draw(void *ptr);
    void LoadRom(FILE *file);

    bool running = false;
};
#endif