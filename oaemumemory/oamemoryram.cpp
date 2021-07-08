#include "headers/oamemoryram.h"

#include <QString>

MemoryRam::MemoryRam(unsigned int size)
{
    memorySize = size;
    memory = new unsigned char[size];
}

MemoryRam::~MemoryRam()
{
    delete[] memory;
}

void MemoryRam::loadData(unsigned char* data)
{
    for (unsigned int i=0; i<memorySize; i++)
    {
        memory[i] = data[i];
    }
}

unsigned char MemoryRam::read(unsigned int location)
{
    if (location >= memorySize)
    {
        throw QString("Reading beyond location of RAM [%1]").arg(location).toLocal8Bit().data(); 
    }
    return memory[location];
}

void MemoryRam::write(unsigned int location, unsigned char byte)
{
    if (location >= memorySize)
    {
        throw QString("Writing beyond location of RAM [%1]").arg(location).toLocal8Bit().data(); 
    }
    memory[location] = byte;
}
