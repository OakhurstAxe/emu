
#include "headers/nescartridge001.h"

namespace oa
{
    namespace nes
    {
        NesCartridge001::NesCartridge001()
        {
            for (int index=0; index<16; index++)
            {
                cpuPrgRom_[index] = new emu::MemoryRom(0x4000, "Cartridge Program ROM");
            }
            for (int index=0; index<2; index++)
            {
                ppuCharRom_[index] = new emu::MemoryRom(0x2000, "PPU Character ROM");
            }
        }
        
        NesCartridge001::~NesCartridge001()
        {
            for (int index=0; index<16; index++)
            {
                delete cpuPrgRom_[index];
            }
            for (int index=0; index<2; index++)
            {
                delete ppuCharRom_[index];
            }
        }
        
        uint8_t NesCartridge001::CpuRead(uint16_t location)
        {
            if (location < 0xc000)
            {
                location -= 0x8000;
                return cpuPrgRom_[cpuProgRomLowerBlock_]->Read(location);
            }
            else
            {
                location -= 0xc000;
                return cpuPrgRom_[1]->Read(location);
            }
        }
        
        void NesCartridge001::CpuWrite(uint16_t location, uint8_t byte)
        {
            Q_UNUSED(location);
            if ((byte & 0x80) > 0)
            {
                cpuProgRomBufferRegister_ = 0;
                cpuProgRomBufferCounter_ = 0;
            }
            else
            {
                cpuProgRomBufferCounter_++;
                if (cpuProgRomBufferCounter_ <= 4)
                {
                    cpuProgRomBufferRegister_ = cpuProgRomBufferRegister_ >> 1;
                    if (byte & 0x01)
                    {
                        cpuProgRomBufferRegister_ = cpuProgRomBufferRegister_ | 0x08;
                    }
                }
                if (cpuProgRomBufferCounter_ == 5)
                {
                    cpuProgRomBufferRegister_ = cpuProgRomBufferRegister_ >> 1;
                    cpuProgRomBufferCounter_ = 0;
                    cpuProgRomLowerBlock_ = 0;
                }
            }
        }
        
        uint8_t NesCartridge001::PpuRead(uint16_t location)
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
        
        void NesCartridge001::PpuWrite(uint16_t location, uint8_t byte)
        {
            Q_UNUSED(byte);
            throw std::out_of_range(QString("Cannot write to Cart PPU ROM %1").arg(location).toLocal8Bit().data());
        }

        void NesCartridge001::LoadProgRom(uint8_t* data, uint8_t size)
        {
            cpuProgRomBlockCount_ = size;
            for (int index=0; index<size; index++)
            {
                cpuPrgRom_[index]->LoadData(&data[index*0x4000], 0x4000);
            }
            cpuProgRomLowerBlock_ = 0;
            cpuProgRomUpperBlock_ = cpuProgRomBlockCount_ - 1;
        }
        
        void NesCartridge001::LoadCharRom(uint8_t* data, uint8_t size)
        {
            if (size == 1)
            {
                ppuCharRom_[0]->LoadData(&data[0], 0x2000);
                ppuCharRom_[1]->LoadData(&data[0], 0x2000);
            }
            else if (size == 2)
            {
                ppuCharRom_[0]->LoadData(&data[0], 0x2000);
                ppuCharRom_[1]->LoadData(&data[0x2000], 0x2000);
            }
        }

        uint16_t NesCartridge001::GetMapper()
        {
            return 0;
        }
        
    }
}
