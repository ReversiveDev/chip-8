#include "vm.hpp"

VM::VM() : ram(new Ram(4096)), cpu(new Cpu(this))
{
    video_memory.resize(64 * 32);
}