#include "chip8.hpp"

const char font[80] = {
    0xf0,
    0x90,
    0x90,
    0x90,
    0xf0,
    0x20,
    0x60,
    0x20,
    0x20,
    0x70,
    0xf0,
    0x10,
    0xf0,
    0x80,
    0xf0,
    0xf0,
    0x10,
    0xf0,
    0x10,
    0xf0,
    0x90,
    0x90,
    0xf0,
    0x10,
    0x10,
    0xf0,
    0x80,
    0xf0,
    0x10,
    0xf0,
    0xf0,
    0x80,
    0xf0,
    0x90,
    0xf0,
    0xf0,
    0x10,
    0x20,
    0x40,
    0x40,
    0xf0,
    0x90,
    0xf0,
    0x90,
    0xf0,
    0xf0,
    0x90,
    0xf0,
    0x10,
    0xf0,
    0xf0,
    0x90,
    0xf0,
    0x90,
    0x90,
    0xe0,
    0x90,
    0xe0,
    0x90,
    0xe0,
    0xf0,
    0x80,
    0x80,
    0x80,
    0xf0,
    0xe0,
    0x90,
    0x90,
    0x90,
    0xe0,
    0xf0,
    0x80,
    0xf0,
    0x80,
    0xf0,
    0xf0,
    0x80,
    0xf0,
    0x80,
    0x80,
};

Chip8::Chip8()
{
    cpu->instruction_set.push_back(Instruction(0xFFFF, 0x00E0, "CLS"));
    cpu->instruction_set.push_back(Instruction(0xFFFF, 0x00EE, "RET"));
    cpu->instruction_set.push_back(Instruction(0xF000, 0x0000, "SYS NNN"));
    cpu->instruction_set.push_back(Instruction(0xF000, 0x2000, "CALL NNN"));
    cpu->instruction_set.push_back(Instruction(0xF000, 0x1000, "JP NNN"));
    cpu->instruction_set.push_back(Instruction(0xF000, 0xB000, "JP V0, NNN"));
    cpu->instruction_set.push_back(Instruction(0xF000, 0x3000, "SE VX, NN"));
    cpu->instruction_set.push_back(Instruction(0xF000, 0x4000, "SNE VX, NN"));
    cpu->instruction_set.push_back(Instruction(0xF00F, 0x5000, "SE VX, VY"));
    cpu->instruction_set.push_back(Instruction(0xF00F, 0x9000, "SNE VX, VY"));
    cpu->instruction_set.push_back(Instruction(0xF0FF, 0xE09E, "SKP VX"));
    cpu->instruction_set.push_back(Instruction(0xF0FF, 0xE0A1, "SKNP VX"));
    cpu->instruction_set.push_back(Instruction(0xF0FF, 0xF0A0, "LD VX, K"));
    cpu->instruction_set.push_back(Instruction(0xF000, 0x6000, "LD VX, NN"));
    cpu->instruction_set.push_back(Instruction(0xF00F, 0x8000, "LD VX, VY"));
    cpu->instruction_set.push_back(Instruction(0xF0FF, 0xF007, "LD VX, DT"));
    cpu->instruction_set.push_back(Instruction(0xF0FF, 0xF00A, "LD VX, K"));
    cpu->instruction_set.push_back(Instruction(0xF0FF, 0xF015, "LD DT, VX"));
    cpu->instruction_set.push_back(Instruction(0xF0FF, 0xF018, "LD ST, VX"));
    cpu->instruction_set.push_back(Instruction(0xF000, 0xA000, "LD I, NNN"));
    cpu->instruction_set.push_back(Instruction(0xF0FF, 0xF029, "LD F, VX"));
    cpu->instruction_set.push_back(Instruction(0xF0FF, 0xF055, "LD [I], VX"));
    cpu->instruction_set.push_back(Instruction(0xF0FF, 0xF065, "LD VX, [I]"));
    cpu->instruction_set.push_back(Instruction(0xF0FF, 0xF01E, "ADD I, VX"));
    cpu->instruction_set.push_back(Instruction(0xF000, 0x7000, "ADD VX, NN"));
    cpu->instruction_set.push_back(Instruction(0xF00F, 0x8004, "ADD VX, VY"));
    cpu->instruction_set.push_back(Instruction(0xF00F, 0x8005, "SUB VX, VY"));
    cpu->instruction_set.push_back(Instruction(0xF00F, 0x8007, "SUBN VX, VY"));
    cpu->instruction_set.push_back(Instruction(0xF00F, 0x8001, "OR VX, VY"));
    cpu->instruction_set.push_back(Instruction(0xF00F, 0x8002, "AND VX, VY"));
    cpu->instruction_set.push_back(Instruction(0xF00F, 0x8003, "XOR VX, VY	"));
    cpu->instruction_set.push_back(Instruction(0xF00F, 0x8006, "SHR VX"));
    cpu->instruction_set.push_back(Instruction(0xF00F, 0x800E, "SHL VX"));
    cpu->instruction_set.push_back(Instruction(0xF0FF, 0xF033, "BCD VX"));
    cpu->instruction_set.push_back(Instruction(0xF000, 0xC000, "RND VX, NN"));
    cpu->instruction_set.push_back(Instruction(0xF000, 0xD000, "DRW VX, VY, N"));
}

int Chip8::Draw(void *ptr)
{
    Chip8 *self = (Chip8 *)ptr;
    int SCREEN_WIDTH = 640;
    int SCREEN_HEIGHT = 320;

    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        window = SDL_CreateWindow("Chip-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            screenSurface = SDL_GetWindowSurface(window);

            SDL_Event e;
            while (self->running)
            {
                SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));

                for (int i = 0; i < self->video_memory.size(); i++)
                {
                    int x = i % 64;
                    int y = i / 64;
                    SDL_Rect rect = {
                        x * 10,
                        y * 10,
                        10,
                        10};
                    if (self->video_memory[i])
                    {
                        SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 255, 255, 255));
                    }
                    else
                    {
                        SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 0, 0));
                    }
                }

                SDL_UpdateWindowSurface(window);

                while (SDL_PollEvent(&e))
                {
                    if (e.type == SDL_QUIT)
                        self->running = false;
                }
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void Chip8::LoadRom(FILE *file)
{
    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    printf("File size: %liB\n", filesize);

    memcpy(ram->memory.data(), font, 80);

    cpu->pc = 0x200;

    fseek(file, 0, SEEK_SET);
    fread(ram->memory.data() + cpu->pc, 1, filesize, file);

    running = true;
    SDL_CreateThread(Draw, "Draw", this);

    while (running)
    {
        if (!cpu->cycle())
        {
            cpu->pc -= 2;
            printf("Error: %i\n", cpu->getOpcode());
            break;
        }

        if (cpu->pc >= 0x200 + filesize)
            break;

        if (cpu->delay_timer > 0)
            cpu->delay_timer--;

        SDL_Delay(1000 / 300);
    }
}