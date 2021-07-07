#ifndef _MEMORYROM_H
#define _MEMORYROM_H

#include "basememory.h"

class MemoryRom : BaseMemory
{
public:
    MemoryRom(unsigned int size);
    virtual ~MemoryRom();
    void loadData(unsigned char* data);
    unsigned char read(unsigned int location);
    void write(unsigned int location, unsigned char byte);
protected:
};

#endif
