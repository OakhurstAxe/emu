#ifndef _NESMEMORY_H
#define _NESMEMORY_H

#include "memoryram.h"
#include "memoryrom.h"

class NesMemory
{
public:
    NesMemory();
    ~NesMemory();
    void loadProgRom(unsigned char* data);
    unsigned char cpuRead(unsigned short location);
    void cpuWrite(unsigned short location, unsigned char byte);
    void cpuSetVblank(unsigned char value);
    
    void loadCharRom(unsigned char* data);
    unsigned char ppuRead(unsigned short location);
    unsigned char ppuOamRead(unsigned short location);
    void ppuWrite(unsigned short location, unsigned char byte);
    unsigned char leftController = 0;
    unsigned char loadController = 0;
private:
    MemoryRam* cpuWorkRam;
    MemoryRam* cpuPpuRegisters;
    MemoryRam* cpuApuIoRegisters;
    MemoryRom* cpuPrgRom;
    MemoryRom* cpuCartridgeRom;
    
    MemoryRom* ppuCharRom;
    MemoryRam* ppuNameTable;
    MemoryRam* ppuPalette;
    MemoryRam* ppuOam;
    
    unsigned char ppuAddrH;
    unsigned char ppuAddrL;
    unsigned short ppuAddr;
    int ppuAddrCount = 0;
    
    unsigned char ppuOamAddr = 0;
};

#endif
