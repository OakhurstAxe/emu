
#include "headers/nesmemory.h"

namespace oa
{
    namespace nes
    {
        
        NesMemory::NesMemory()
        {
            cpuWorkRam_ = new emu::MemoryRam(0x0800, "CPU Work RAM");
            cpuPpuRegisters_ = new emu::MemoryRam(0x0008, "PPU Registers");
            for (int index=0; index<16; index++)
            {
                cpuPrgRom_[index] = new emu::MemoryRom(0x4000, "CPU Program ROM");
            }
            cpuApuIoRegisters_ = new emu::MemoryRamFlagged(0x001f, "APU IO Registers");
            
            ppuCharRom_ = new emu::MemoryRom(0x4000, "PPU Character ROM");
            ppuNameTable_ = new emu::MemoryRam(0x1f00, "PPU Name Table RAM");
            ppuPalette_ = new emu::MemoryRam(0x0100, "PPU Palette RAM");
            ppuOam_ = new emu::MemoryRam(0x0100, "PPU OAM RAM");
        }

        NesMemory::~NesMemory()
        {
            delete cpuWorkRam_;
            delete cpuPpuRegisters_;
            for (int index=0; index<16; index++)
            {
                delete cpuPrgRom_[index];
            }
            delete cpuApuIoRegisters_;
            
            delete ppuCharRom_;
            delete ppuNameTable_;
            delete ppuPalette_;
            delete ppuOam_;
        }

        void NesMemory::SetPpuScanLineStatus(uint16_t scanLine)
        {
            uint8_t byte = (cpuPpuRegisters_->Read(2) & 0xe0) + (scanLine & 0x7f);
            cpuPpuRegisters_->Write(2, byte);
        }
        
        void NesMemory::SetPpuSpriteOvervlow()
        {
            uint8_t byte = cpuPpuRegisters_->Read(2) | 0x20;
            cpuPpuRegisters_->Write(2, byte);
        }

        void NesMemory::SetPpuSpriteZeroHit()
        {
            uint8_t byte = cpuPpuRegisters_->Read(2) | 0x40;
            cpuPpuRegisters_->Write(2, byte);
        }

        uint8_t NesMemory::CpuRead(uint16_t location)
        {
            uint16_t originalLocation = location;
            
            // Working RAM
            if (location < 0x2000)
            {
                location = location % 0x800;  // mirroring
                return cpuWorkRam_->Read(location);
            }
            
            // PPU Registers
            else if (location < 0x4000)
            {
                // Mirroring, and bring to zero
                location = location % 8;
                if (location == 2)
                {
                    uint8_t byte = cpuPpuRegisters_->Read(2);
                    byte = byte & 0xd9;
                    cpuPpuRegisters_->Write(2,  byte);
                    ppuAddrCount_ = 0;
                }
                if (location == 5)
                {
                    if (ppuXScrollRead_)
                    {
                        ppuXScrollRead_ = false;
                        return ppuXScroll_;
                    }
                    else
                    {
                        ppuXScroll_ = true;
                        return ppuYScroll_;
                    }
                }
                return cpuPpuRegisters_->Read(location);
            }    

            // APU and IO Registers
            else if (location < 0x4020)
            {
                location -= 0x4000;
                if (location == 0x16)
                {
                    uint8_t result = ((leftController_ & 0x01) > 0);
                    leftController_ >>= 1;
                    return result;
                }
                if (location == 0x17)
                {
                    uint8_t result = ((rightController_ & 0x01) > 0);
                    rightController_ >>= 1;
                    return result;
                }
                return cpuApuIoRegisters_->Read(location);
            }
            
            else if (location < 0x8000)
            {
                throw std::out_of_range(QString("CPU Read in unknown area %1").arg(originalLocation).toLocal8Bit().data());
            }
            
            // ROM
            else
            {
                location -= 0x8000;
                if (location < 0x4000)
                {
                    return cpuPrgRom_[cpuProgRomLowerBlock_]->Read(location);
                }
                else
                {
                    location -= 0x4000;
                    return cpuPrgRom_[cpuProgRomUpperBlock_]->Read(location);
                }
            }
            
            throw std::out_of_range(QString("Invalid NES memory location for read %1").arg(originalLocation).toLocal8Bit().data());
        }

        void NesMemory::CpuWrite(uint16_t location, uint8_t byte)
        {
            uint16_t originalLocation = location;
            
            // Working RAM
            if (location < 0x2000)
            {
                if (location == 583)
                {
                    int x = 10;
                }
                location = location % 0x800;  // mirroring
                cpuWorkRam_->Write(location, byte);
                return;
            }
            
            // PPU Registers
            else if (location < 0x4000)
            {
                // Mirroring, and bring to zero
                location = location % 8;
                cpuPpuRegisters_->Write(location, byte);

                if (location == 03)
                {
                    ppuOamAddr_ = byte;
                }
                if (location == 0x4)
                {
                    ppuOam_->Write(ppuOamAddr_, byte);
                    ppuOamAddr_++;
                }
                if (location == 0x5)
                {
                    if (ppuXScrollWrite_)
                    {
                        ppuXScroll_ = byte;
                        ppuXScrollWrite_ = false;
                    }
                    else
                    {
                        ppuYScroll_ = byte;
                        ppuXScrollWrite_ = true;
                    }
                }
                if (location == 0x6)
                {
                    ppuAddrCount_++;
                    if (ppuAddrCount_ == 1)
                    {
                        ppuAddrH_ = byte;
                    }
                    if (ppuAddrCount_ == 2)
                    {
                        ppuAddrL_ = byte;
                        ppuAddr_ = (ppuAddrH_ << 8) + ppuAddrL_;
                        ppuAddrCount_ = 0;
                    }
                }
                
                if (location == 0x7 && ppuAddr_ != 0)
                {
                    PpuWrite(ppuAddr_, byte);
                    uint8_t controller = CpuRead(0x2000);
                    if (controller & 0x04)
                    {
                        ppuAddr_ += 32;
                    }
                    else
                    {
                        ppuAddr_++;
                    }
                }
                return;
            }
            
            // APU and IO Registers            
            else if (location < 0x4020)
            {
                if (location == 0x4014)
                {
                    uint16_t cpuAddr = byte << 8;
                    for (int i=0; i<256; i++)
                    {
                        uint8_t spriteData = CpuRead(cpuAddr + i);
                        ppuOam_->Write(i, spriteData);
                    }
                    return;
                }
                
                location -= 0x4000;
                cpuApuIoRegisters_->Write(location, byte);
                return;
            }
            
            else if (location < 0x8000)
            {
                throw std::out_of_range(QString("CPU Write in unknown area %1").arg(originalLocation).toLocal8Bit().data());
            }
            
            // ROM
            else
            {
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
                        cpuProgRomLowerBlock_ = 0;// cpuProgRomBufferRegister_;
                    }
                }
                return;
            }

            throw std::out_of_range(QString("Invalid NES memory location for write %1").arg(originalLocation).toLocal8Bit().data());
        }

        bool NesMemory::CpuReadFlagged(uint16_t location)
        {
            if (location < 0x4000)
            {
                throw std::out_of_range(QString("Bad location address [%1] for read flag").arg(location).toLocal8Bit().data());
            }            
            else if (location < 0x4020)
            {
                location -= 0x4000;
                return cpuApuIoRegisters_->IsReadFlagSet(location);
            }
            throw std::out_of_range(QString("Bad location address [%1] for read flag").arg(location).toLocal8Bit().data());
        }

        bool NesMemory::CpuWriteFlagged(uint16_t location)
        {
            if (location < 0x4000)
            {
                throw std::out_of_range(QString("Bad location address [%1] for write flag").arg(location).toLocal8Bit().data());
            }            
            else if (location < 0x4020)
            {
                location -= 0x4000;
                return cpuApuIoRegisters_->IsWriteFlagSet(location);
            }
            throw std::out_of_range(QString("Bad location address [%1] for write flag").arg(location).toLocal8Bit().data());
        }

        uint8_t NesMemory::PpuRead(uint16_t location)
        {
            if (location < 0x2000)
            {
                return ppuCharRom_->Read(location);        
            }
            
            else if (location < 0x3f00)
            {
                location -= 0x2000;
                return ppuNameTable_->Read(location);
            }
            
            else if (location < 0x4000)
            {
                location -= 0x3f00;
                if ((location % 4) == 0)
                {
                    ppuPalette_->Read(0);
                }

                return ppuPalette_->Read(location);
            }
            
            throw std::out_of_range("Invalid read address");
        }

        void NesMemory::PpuWrite(uint16_t location, uint8_t byte)
        {
            if (location < 0x2000)
            {
                ppuCharRom_->Write(location, byte); 
                return;
            }
            
            else if (location < 0x3f00)
            {
                location -= 0x2000;
                ppuNameTable_->Write(location, byte);
                return;
            }
            
            else if (location < 0x4000)
            {
                location -= 0x3f00;
                if ((location % 4) == 0)
                {
                    ppuPalette_->Write(0, byte);
                }
                ppuPalette_->Write(location, byte);
                return;
            }
            
            throw std::out_of_range(QString("Invalid NES memory location for PPU write %1").arg(location).toLocal8Bit().data());
        }

        uint8_t NesMemory::PpuOamRead(uint16_t location)
        {
            if (location < 256)
            {
                return ppuOam_->Read(location);
            }
            throw std::out_of_range(QString("Invalid OAM memory location for PPU read %1").arg(location).toLocal8Bit().data());
        
        }
        
        void NesMemory::CpuSetVblank(uint8_t value)
        {
            uint8_t byte = cpuPpuRegisters_->Read(2);
            if (value == 1)
            {
                byte |= 0x80;
            }
            else
            {
                byte &= 0x7f;
            }
            cpuPpuRegisters_->Write(2, byte);
        }
        
        void NesMemory::SetLeftController(uint8_t byte)
        {
            leftController_ = byte;
        }

        void NesMemory::SetRightController(uint8_t byte)
        {
            rightController_ = byte;
        }

        void NesMemory::LoadProgRom(uint8_t* data, uint8_t size)
        {
            cpuProgRomBlockCount_ = size;
            for (int index=0; index<size; index++)
            {
                cpuPrgRom_[index]->LoadData(&data[index*0x4000], 0x4000);
            }
            cpuProgRomLowerBlock_ = 0;
            cpuProgRomUpperBlock_ = cpuProgRomBlockCount_ - 1;
        }

        void NesMemory::LoadCharRom(uint8_t* data, uint8_t size)
        {
            ppuCharRom_->LoadData(data, size * 0x2000);
        }

    }
}
