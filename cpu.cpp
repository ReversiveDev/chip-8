#include "cpu.hpp"

Cpu::Cpu(VM *vm) : vm(vm)
{
    registers.resize(16);
    stack.resize(16);
}

uint16_t Cpu::getOpcode(int offset)
{
    if (offset < 0)
        offset = pc;
    return (vm->ram->memory[offset] << 8) | vm->ram->memory[offset + 1];
}

Instruction *Cpu::fetch()
{
    uint16_t opcode = getOpcode();
    pc += 2;
    for (int i = 0; i < instruction_set.size(); i++)
    {
        if (instruction_set[i].combine(opcode))
        {
            return &instruction_set[i];
        }
    }
    return NULL;
}

bool Cpu::cycle()
{
    Instruction *instruction;
    if ((instruction = fetch()) == NULL)
    {
        printf("Error: Invalid instruction opcode.\n");
        return false;
    }
    else
    {
        // printf("%s\n", instruction->name);
    }

    uint16_t opcode = getOpcode(pc - 2);
    uint16_t nnn = opcode & 0xFFF;     // address
    uint8_t nn = opcode & 0xFF;        // 8-bit value
    uint8_t n = opcode & 0xF;          // 4-bit value
    uint8_t x = (opcode & 0xF00) >> 8; // x register
    uint8_t y = (opcode & 0xF0) >> 4;  // y register

    if (strcmp(instruction->name, "CLS") == 0)
    {
        for (int i = 0; i < vm->video_memory.size(); i++)
            vm->video_memory[i] = 0;
    }
    else if (strcmp(instruction->name, "LD I, NNN") == 0)
    {
        i = nnn;
    }
    else if (strcmp(instruction->name, "LD VX, NN") == 0)
    {
        registers[x] = nn;
    }
    else if (strcmp(instruction->name, "DRW VX, VY, N") == 0)
    {
        registers[15] = 0;
        int width = 8;
        for (int i = 0; i < n; i++)
        {
            uint8_t membyte = vm->ram->memory[this->i + i];
            for (int j = 0; j < width; j++)
            {
                if (membyte & (0x80 >> j))
                {
                    uint8_t px = registers[x] + j;
                    uint8_t py = registers[y] + i;
                    uint8_t pixel = vm->video_memory[px + py * 64];
                    if (pixel == 1)
                    {
                        registers[15] = 1;
                    }
                    vm->video_memory[px + py * 64] ^= 1;
                }
            }
        }
    }
    else if (strcmp(instruction->name, "ADD VX, NN") == 0)
    {
        registers[x] = (registers[x] + nn) & 0xFF;
    }
    else if (strcmp(instruction->name, "JP NNN") == 0)
    {
        pc = nnn;
    }
    else if (strcmp(instruction->name, "CALL NNN") == 0)
    {
        stack[sp++] = pc;
        pc = nnn;
    }
    else if (strcmp(instruction->name, "BCD VX") == 0)
    {
        vm->ram->memory[i + 0] = registers[x] / 100 % 10;
        vm->ram->memory[i + 1] = registers[x] / 10 % 10;
        vm->ram->memory[i + 2] = registers[x] / 1 % 10;
    }
    else if (strcmp(instruction->name, "LD VX, [I]") == 0)
    {
        for (int i = 0; i <= x; i++)
        {
            registers[i] = vm->ram->memory[this->i + i];
        }
    }
    else if (strcmp(instruction->name, "LD F, VX") == 0)
    {
        i = registers[x] * 5;
    }
    else if (strcmp(instruction->name, "RET") == 0)
    {
        pc = stack[--sp];
    }
    else if (strcmp(instruction->name, "LD DT, VX") == 0)
    {
        delay_timer = registers[x];
    }
    else if (strcmp(instruction->name, "LD VX, DT") == 0)
    {
        registers[x] = delay_timer;
    }
    else if (strcmp(instruction->name, "SE VX, NN") == 0)
    {
        if (registers[x] == nn)
        {
            pc += 2;
        }
    }
    else if (strcmp(instruction->name, "RND VX, NN") == 0)
    {
        srand(time(0));
        // rand() % (max - min + 1) + min;
        registers[x] = (rand() % 256) & nn;
    }
    else if (strcmp(instruction->name, "SKNP VX") == 0)
    {
        // TODO Skip if not pressed
        pc += 2;
    }
    else if (strcmp(instruction->name, "AND VX, VY") == 0)
    {
        registers[x] &= registers[y];
    }
    else if (strcmp(instruction->name, "ADD VX, VY") == 0)
    {
        if (registers[x] + registers[y] > 255)
            registers[15] = 1;
        else
            registers[15] = 0;
        registers[x] = (registers[x] + registers[y]) & 0xFF;
    }
    else if (strcmp(instruction->name, "SNE VX, NN") == 0)
    {
        if (registers[x] != nn)
            pc += 2;
    }
    else if (strcmp(instruction->name, "LD VX, VY") == 0)
    {
        registers[x] = registers[y];
    }
    else if (strcmp(instruction->name, "SUB VX, VY") == 0)
    {
        if (registers[x] > registers[y])
            registers[15] = 1;
        else
            registers[15] = 0;
        registers[x] = (registers[x] - registers[y]) & 0xFF;
    }
    else if (strcmp(instruction->name, "LD ST, VX") == 0)
    {
        // Sound TODO.
    }
    else if (strcmp(instruction->name, "SUBN VX, VY") == 0)
    {
        if (registers[x] > registers[y])
            registers[15] = 1;
        else
            registers[15] = 0;
        registers[x] = registers[y] - registers[x];
    }
    else if (strcmp(instruction->name, "SE VX, VY") == 0)
    {
        if (registers[x] == registers[y])
            pc += 2;
    }
    else if (strcmp(instruction->name, "ADD I, VX") == 0)
    {
        i += registers[x];
    }
    else if (strcmp(instruction->name, "SKP VX") == 0)
    {
        // TODO Skip if pressed
    }
    else if (strcmp(instruction->name, "SNE VX, VY") == 0)
    {
        if (registers[x] != registers[y])
            pc += 2;
    }
    else if (strcmp(instruction->name, "SHL VX") == 0)
    {
        registers[15] = registers[x] >> 7;
        registers[x] <<= 1;
    }
    else if (strcmp(instruction->name, "LD [I], VX") == 0)
    {
        for (int i = 0; i <= x; i++)
        {
            vm->ram->memory[this->i + i] = registers[i];
        }
    }
    else if (strcmp(instruction->name, "LD VX, K") == 0)
    {
        // TODO Wait for key press and store in Vx
        pc -= 2;
    }
    else if (strcmp(instruction->name, "SYS NNN") == 0)
    {
        // Ignore
    }
    else
    {
        printf("Instruction not implemented.\n");
        return false;
    }

    return true;
}