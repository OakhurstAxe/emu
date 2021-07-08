#include "headers/oamemoryrom.h"

#include <QString>

MemoryRom::MemoryRom(unsigned int size)
{
    memorySize = size;
    memory = new unsigned char[size];
}

MemoryRom::~MemoryRom()
{
    delete[] memory;
}

void MemoryRom::loadData(unsigned char* data)
{
    for (unsigned int i=0; i<memorySize; i++)
    {
        memory[i] = data[i];
    }
}

unsigned char MemoryRom::read(unsigned int location)
{
    if (location >= memorySize)
    {
        throw QString("Reading beyond location of ROM [%1]").arg(location).toLocal8Bit().data(); 
    }
    return memory[location];
}

void MemoryRom::write(unsigned int location, unsigned char byte)
{
    throw QString("Cannot write to ROM [%1] = %2").arg(location).arg(byte).toLocal8Bit().data(); 
}
