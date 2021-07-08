
#include "headers/oanesmemory.h"

#include <QString>

NesMemory::NesMemory()
{
    cpuWorkRam = new MemoryRam(0x800);
    cpuPpuRegisters = new MemoryRam(0x8);
    cpuPrgRom = new MemoryRom(0x4000);
    cpuApuIoRegisters = new MemoryRam(0x001f);
    cpuCartridgeRom = new MemoryRom(0x1fdf);
    
    ppuCharRom = new MemoryRom(0x2000);
    ppuNameTable = new MemoryRam(0x1f00);
    ppuPalette = new MemoryRam(0x0100);
    ppuOam = new MemoryRam(0x100);
}

NesMemory::~NesMemory()
{
    delete cpuWorkRam;
    delete cpuPpuRegisters;
    delete cpuPrgRom;
    delete cpuApuIoRegisters;
    
    delete ppuCharRom;
    delete ppuNameTable;
    delete ppuPalette;
    delete ppuOam;
}

void NesMemory::loadProgRom(unsigned char* data)
{
    cpuPrgRom->loadData(data);
}

unsigned char NesMemory::cpuRead(unsigned short location)
{
    unsigned short origLocation = location;
    // Working RAM
    if (location < 0x2000)
    {
        location = location % 0x800;  // mirroring
        return cpuWorkRam->read(location);
    }
    
    // ROM
    if (location >= 0x8000)
    {
        // ROM mirroring, and bring to zero
        location = location % 0x4000;
        return cpuPrgRom->read(location);
    }
    
    // PPU Registers
    if (location >=0x2000 && location < 0x4000)
    {
        // Mirroring, and bring to zero
        location = location % 8;
        if (location == 2)
        {
            unsigned char byte = cpuPpuRegisters->read(2);
            byte = byte & 0xd9;
            cpuPpuRegisters->write(2,  byte);
            ppuAddrCount = 0;
        }
        return cpuPpuRegisters->read(location);
    }    
   
    // APU and IO Registers
    if (location >= 0x4000 && location < 0x4020)
    {
        location -= 0x4000;
        if (location == 0x16)
        {
            char result = ((leftController & 0x01) > 0);
            leftController >>= 1;
            return result;
        }
        return cpuApuIoRegisters->read(location);
    }
    
    if (location >= 0x4020 && location <0x6000)
    {
        location -= 0x4020;
        return cpuCartridgeRom->read(location);
    }
    
    throw QString("Invalid NES memory location for read %1").arg(location).toLocal8Bit().data();
}

void NesMemory::cpuSetVblank(unsigned char value)
{
    unsigned char byte = cpuPpuRegisters->read(2);
    if (value == 1)
    {
        byte |= 0x80;
    }
    else
    {
        byte &= 0x7f;
    }
    cpuPpuRegisters->write(2, byte);
}

void NesMemory::cpuWrite(unsigned short location, unsigned char byte)
{
    unsigned short origLocation = location;
    
    // Working RAM
    if (location < 0x2000)
    {
        location = location % 0x800;  // mirroring
        cpuWorkRam->write(location, byte);
        return;
    }
    
    // ROM
    if (location >= 0x8000)
    {
        // ROM mirroring, and bring to zero
        location = location % 0x4000;
        cpuPrgRom->write(location, byte);
        return;
    }
    
    // APU and IO Registers
    
    if (location >= 0x4000 && location < 0x4020)
    {
        if (location == 0x4014)
        {
            unsigned short cpuAddr = byte << 8;
            for (int i=0; i<256; i++)
            {
                unsigned char spriteData = cpuRead(cpuAddr + i);
                ppuOam->write(i, spriteData);
            }
            return;
        }
        
        if (location == 0x4016)
        {
            loadController = 1;
            return;
        }
        
        location -= 0x4000;
        cpuApuIoRegisters->write(location, byte);
        return;
    }
    
    // PPU Registers
    if (location >=0x2000 && location < 0x4000)
    {
        // Mirroring, and bring to zero
        location = location % 8;
        cpuPpuRegisters->write(location, byte);

        if (location == 03)
        {
            ppuOamAddr = byte;
        }
        if (location == 0x4)
        {
            ppuOam->write(ppuOamAddr, byte);
            ppuOamAddr++;
        }
        if (location == 0x6)
        {
            ppuAddrCount++;
            if (ppuAddrCount == 1)
            {
                ppuAddrH = byte;
            }
            if (ppuAddrCount == 2)
            {
                ppuAddrL = byte;
                ppuAddr = (ppuAddrH << 8) + ppuAddrL;
                ppuAddrCount = 0;
            }
        }
        
        if (location == 0x7 && ppuAddr != 0)
        {
            ppuWrite(ppuAddr, byte);
            unsigned char controller = cpuRead(0x2000);
            if (controller & 0x04)
            {
                ppuAddr += 32;
            }
            else
            {
                ppuAddr++;
            }
        }
        return;
    }
    
    // OMA DMA
    if (location == 0x4014)
    {

    }
    
    // APU and IO Registers
    if (location >= 0x4000 && location < 0x4018)
    {
        location -= 0x4000;
        cpuApuIoRegisters->write(location, byte);
        return;
    }

    throw QString("Invalid NES memory location for write %1").arg(location).toLocal8Bit().data();
}

void NesMemory::loadCharRom(unsigned char* data)
{
    ppuCharRom->loadData(data);
}
unsigned char NesMemory::ppuRead(unsigned short location)
{
    unsigned short origLocation = location;

    //location = location & 0x3FFF;
    
    if (location < 0x2000)
    {
        return ppuCharRom->read(location);        
    }
    
    if (location < 0x3f00)
    {
        location -= 0x2000;
        return ppuNameTable->read(location);
    }
    
    if (location < 0x4000)
    {
        location -= 0x3f00;
        if ((location % 4) == 0)
        {
            ppuPalette->read(0);
        }

        return ppuPalette->read(location);
    }
    throw "Invalid read address";
}

void NesMemory::ppuWrite(unsigned short location, unsigned char byte)
{
    unsigned short origLocation = location;
    //location = location % 0x4000;
    
    if (location < 0x2000)
    {
        ppuCharRom->write(location, byte); 
        return;
    }
    
    if (location < 0x3f00)
    {
        location -= 0x2000;
        ppuNameTable->write(location, byte);
        return;
    }
    
    if (location < 0x4000)
    {
        location -= 0x3f00;
        if ((location % 4) == 0)
        {
            ppuPalette->write(0, byte);
        }
        ppuPalette->write(location, byte);
        return;
        //throw "Cannot write to PPU palatte";
    }
    
    QString("Invalid NES memory location for PPU write %1").arg(location).toLocal8Bit().data(); 
}

unsigned char NesMemory::ppuOamRead(unsigned short location)
{
    if (location < 256)
    {
        return ppuOam->read(location);
    }
    QString("Invalid OAM memory location for PPU read %1").arg(location).toLocal8Bit().data(); 
   
}
