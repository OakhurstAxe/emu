
#include "headers/nesmemory.h"

namespace oa
{
    namespace nes
    {
        
        NesMemory::NesMemory()
        {
            cpuWorkRam_ = new emu::MemoryRam(0x0800, "CPU Work RAM");
            cpuPpuRegisters_ = new emu::MemoryRam(0x0008, "PPU Registers");
            cpuPrgRom_ = new emu::MemoryRom(0x4000, "CPU Program ROM");
            cpuApuIoRegisters_ = new emu::MemoryRam(0x001f, "APU IO Registers");
            
            ppuCharRom_ = new emu::MemoryRom(0x2000, "PPU Character ROM");
            ppuNameTable_ = new emu::MemoryRam(0x1f00, "PPU Name Table RAM");
            ppuPalette_ = new emu::MemoryRam(0x0100, "PPU Palette RAM");
            ppuOam_ = new emu::MemoryRam(0x0100, "PPU OAM RAM");
        }

        NesMemory::~NesMemory()
        {
            delete cpuWorkRam_;
            delete cpuPpuRegisters_;
            delete cpuPrgRom_;
            delete cpuApuIoRegisters_;
            
            delete ppuCharRom_;
            delete ppuNameTable_;
            delete ppuPalette_;
            delete ppuOam_;
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
                return cpuPpuRegisters_->Read(location);
            }    

            // APU and IO Registers
            else if (location < 0x4020)
            {
                location -= 0x4000;
                if (location == 0x09)
                {   
                    uint8_t result = cpuApuIoRegisters_->Read(location);
                    cpuApuIoRegisters_->Write(location, 0);
                    return result;
                }

                if (location == 0x16)
                {
                    uint8_t result = ((leftController_ & 0x01) > 0);
                    leftController_ >>= 1;
                    return result;
                }
                return cpuApuIoRegisters_->Read(location);
            }
            
            else if (location < 0x8000)
            {
                throw std::out_of_range(QString("CPU Read in battery backup area %1").arg(originalLocation).toLocal8Bit().data());
            }
            
            // ROM
            else
            {
                // ROM mirroring, and bring to zero
                location = location % 0x4000;
                return cpuPrgRom_->Read(location);
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

                if (location == 03)
                {
                    ppuOamAddr_ = byte;
                }
                if (location == 0x4)
                {
                    ppuOam_->Write(ppuOamAddr_, byte);
                    ppuOamAddr_++;
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
                if (location == 0x4008)
                {
                    cpuApuIoRegisters_->Write(0x0009, 1);
                }
                    
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
                
                if (location == 0x4016)
                {
                    loadController_ = 1;
                    return;
                }
                
                location -= 0x4000;
                cpuApuIoRegisters_->Write(location, byte);
                return;
            }
            
            else if (location < 0x8000)
            {
                throw std::out_of_range(QString("CPU Read in battery backup area %1").arg(originalLocation).toLocal8Bit().data());
            }
            
            // ROM
            else
            {
                // ROM mirroring, and bring to zero
                location = location % 0x4000;
                cpuPrgRom_->Write(location, byte);
                return;
            }

            throw std::out_of_range(QString("Invalid NES memory location for write %1").arg(originalLocation).toLocal8Bit().data());
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
        
        bool NesMemory::IsLoadController()
        {
            bool result = loadController_;
            loadController_ = false;
            return result;
        }
        
        void NesMemory::SetLeftController(uint8_t byte)
        {
            leftController_ = byte;
        }

        void NesMemory::LoadProgRom(uint8_t* data, uint16_t size)
        {
            cpuPrgRom_->LoadData(data, size);
        }

        void NesMemory::LoadCharRom(uint8_t* data, uint16_t size)
        {
            ppuCharRom_->LoadData(data, size);
        }

    }
}
