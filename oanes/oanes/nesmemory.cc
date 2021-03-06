
#include "headers/nesmemory.h"

namespace oa
{
    namespace nes
    {
        
        NesMemory::NesMemory()
        {
            cpuWorkRam_ = new emu::MemoryRam(0x0800, "CPU Work RAM");
            cpuPpuRegisters_ = new emu::MemoryRam(0x0008, "PPU Registers");
            cpuApuIoRegisters_ = new emu::MemoryRamFlagged(0x001f, "APU IO Registers");
            
            ppuNameTable_ = new emu::MemoryRam(0x1f00, "PPU Name Table RAM");
            ppuPalette_ = new emu::MemoryRam(0x0100, "PPU Palette RAM");
            ppuOam_ = new emu::MemoryRam(0x0100, "PPU OAM RAM");
        }

        NesMemory::~NesMemory()
        {
            delete cpuWorkRam_;
            delete cpuPpuRegisters_;
            delete cpuApuIoRegisters_;
            
            delete ppuNameTable_;
            delete ppuPalette_;
            delete ppuOam_;
        }

        void NesMemory::SetCartridge(NesCartridge* cartridge)
        {
            cartridge_ = cartridge;
        }
        
        void NesMemory::SetCpu(R2A03* cpu)
        {
            cpu_ = cpu;
        }

        void NesMemory::SetPpuSpriteOvervlow(uint8_t value)
        {
            uint8_t byte;
            if (value > 0)
            {
                byte = cpuPpuRegisters_->Read(2) | 0x20;
            }
            else
            {
                byte = cpuPpuRegisters_->Read(2) & 0xdf;
            }
            cpuPpuRegisters_->Write(2, byte);
        }

        void NesMemory::SetPpuSpriteZeroHit(uint8_t value)
        {
            uint8_t byte;
            if (value > 0)
            {
                byte = cpuPpuRegisters_->Read(2) | 0x40;
            }
            else
            {
                byte = cpuPpuRegisters_->Read(2) & 0xbf;
            }
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
                if (location == 0x02)
                {
                    uint8_t byte = cpuPpuRegisters_->Read(2);
                    cpuPpuRegisters_->Write(2,  byte & 0x60);
                    ppuAddrCount_ = 0;
                    return byte;
                }
                if (location == 0x05)
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
            
            // Cartridge RAM/ROM
            else
            {
                return cartridge_->CpuRead(location);
            }
            
            throw std::out_of_range(QString("Invalid NES memory location for read %1").arg(originalLocation).toLocal8Bit().data());
        }

        void NesMemory::CpuWrite(uint16_t location, uint8_t byte)
        {
            uint16_t originalLocation = location;
            
            // Working RAM
            if (location < 0x2000)
            {
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

                if (location == 0x03)
                {
                    ppuOamAddr_ = byte;
                }
                if (location == 0x04)
                {
                    ppuOam_->Write(ppuOamAddr_, byte);
                    ppuOamAddr_++;
                }
                if (location == 0x05)
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
                if (location == 0x06)
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
                
                if (location == 0x07)// && ppuAddr_ != 0)
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
                    cpu_->DmaSuspend();
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
            
            // Cartridge RAM/ROM
            else
            {
                cartridge_->CpuWrite(location, byte);
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
            //  Cartridge PPU ROM
            if (location < 0x2000)
            {
                return cartridge_->PpuRead(location);
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
            // Cartridge PPU ROM
            if (location < 0x2000)
            {
                cartridge_->PpuWrite(location, byte);
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

    }
}
