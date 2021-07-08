#ifndef _MEMORYRAM_H
#define _MEMORYRAM_H

#include "oabasememory.h"

class MemoryRam : BaseMemory
{
public:
    MemoryRam(unsigned int size);
    virtual ~MemoryRam();
    void loadData(unsigned char* data);
    unsigned char read(unsigned int location);
    void write(unsigned int location, unsigned char byte);
protected:
};

#endif

