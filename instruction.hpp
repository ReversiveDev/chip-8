#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <stdint.h>

class Instruction
{
public:
    Instruction(uint16_t mask, uint16_t pattern, const char* name);

    bool combine(uint16_t opcode);

    uint16_t mask;
    uint16_t pattern;
    const char* name;
};

#endif