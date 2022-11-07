#ifndef RAM_H
#define RAM_H
#include <stdint.h>
#include <vector>

class Ram
{
private:
public:
    std::vector<uint8_t> memory;
    Ram(int memorySize);
};

#endif