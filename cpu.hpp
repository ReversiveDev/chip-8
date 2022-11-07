#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <random>
#include "instruction.hpp"

class Cpu;

#include "vm.hpp"

class Cpu
{
private:
    VM *vm;

public:
    Cpu(VM *vm);

    uint16_t getOpcode(int offset = -1);
    Instruction* fetch();
    bool cycle();

    std::vector<Instruction> instruction_set;
    std::vector<uint8_t> registers;
    std::vector<uint16_t> stack;
    uint16_t pc = 0;
    uint16_t sp = 0;
    uint16_t i = 0;
    uint16_t delay_timer = 0;
};
#endif