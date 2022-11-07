#include "instruction.hpp"

Instruction::Instruction(uint16_t mask, uint16_t pattern, const char *name) : mask(mask), pattern(pattern), name(name)
{
}

bool Instruction::combine(uint16_t opcode)
{
    return (opcode & mask) == pattern;
}