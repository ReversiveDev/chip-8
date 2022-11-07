#ifndef VM_H
#define VM_H

class VM;

#include <memory>
#include "ram.hpp"
#include "cpu.hpp"
#include "instruction.hpp"

class VM
{
public:
    VM();

    std::unique_ptr<Ram> ram;
    std::unique_ptr<Cpu> cpu;
    std::vector<uint8_t> video_memory;
};

#endif