#ifndef _BASEMEMORY_H
#define _BASEMEMORY_H

class BaseMemory
{
public:
    virtual unsigned char read(unsigned int location) = 0;
    virtual void write(unsigned int location, unsigned char byte) = 0;
protected:
    unsigned int memorySize = 0;
    unsigned int startAddress = 0;
    unsigned char* memory;
};

#endif
