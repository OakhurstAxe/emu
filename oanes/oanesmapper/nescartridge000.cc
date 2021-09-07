
#include "headers/nescartridge000.h"

namespace oa
{
    namespace nes
    {
        NesCartridge000::NesCartridge000()
        {
            for (int index=0; index<2; index++)
            {
                cpuPrgRom_[index] = new emu::MemoryRom(0x4000, "Cartridge Program ROM");
                ppuCharRom_[index] = new emu::MemoryRom(0x2000, "PPU Character ROM");
            }
        }
        
        NesCartridge000::~NesCartridge000()
        {
            for (int index=0; index<2; index++)
            {
                delete cpuPrgRom_[index];
                delete ppuCharRom_[index];
            }
        }
        
        uint8_t NesCartridge000::CpuRead(uint16_t location)
        {
            if (location < 0xc000)
            {
                location -= 0x8000;
                return cpuPrgRom_[0]->Read(location);
            }
            else
            {
                location -= 0xc000;
                return cpuPrgRom_[1]->Read(location);
            }
        }
        
        void NesCartridge000::CpuWrite(uint16_t location, uint8_t byte)
        {
            Q_UNUSED(byte);
            throw std::out_of_range(QString("Cannot write to Cart ROM %1").arg(location).toLocal8Bit().data());
        }
        
        uint8_t NesCartridge000::PpuRead(uint16_t location)
        {
            if (location < 0x2000)
            {
                return ppuCharRom_[0]->Read(location);
            }
            else
            {
                location -= 0x2000;
                return ppuCharRom_[1]->Read(location);
            }
        }
        
        void NesCartridge000::PpuWrite(uint16_t location, uint8_t byte)
        {
            Q_UNUSED(byte);
            throw std::out_of_range(QString("Cannot write to Cart PPU ROM %1").arg(location).toLocal8Bit().data());
        }

        void NesCartridge000::LoadProgRom(uint8_t* data, uint8_t size)
        {
            if (size == 1)
            {
                cpuPrgRom_[0]->LoadData(&data[0], 4);
                cpuPrgRom_[1]->LoadData(&data[0], 4);
            }
            else if (size == 2)
            {
                cpuPrgRom_[0]->LoadData(&data[0], 4);
                cpuPrgRom_[1]->LoadData(&data[0x4000], 4);
            }
        }
        
        void NesCartridge000::LoadCharRom(uint8_t* data, uint8_t size)
        {
            if (size == 1)
            {
                ppuCharRom_[0]->LoadData(&data[0], 2);
                ppuCharRom_[1]->LoadData(&data[0], 2);
            }
            else if (size == 2)
            {
                ppuCharRom_[0]->LoadData(&data[0], 0x2000);
                ppuCharRom_[1]->LoadData(&data[0x2000], 0x2000);
            }
        }

        uint16_t NesCartridge000::GetMapper()
        {
            return 0;
        }
        
    }
}
