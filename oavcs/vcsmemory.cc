
#include "headers/vcsmemory.h"

namespace oa
{
    namespace vcs
    {
        
        VcsMemory::VcsMemory(VcsTia *vcsTia, emu::MemoryRam *ram, VcsRiot *vcsRiot, VcsCartridge *vcsCartridge)
        {
            vcsTia_ = vcsTia;
            ram_ = ram;
            vcsRiot_ = vcsRiot;
            vcsCartridge_ = vcsCartridge;
        }

        VcsMemory::~VcsMemory()
        {
        }

        uint8_t VcsMemory::CpuRead(uint16_t location)
        {
            uint16_t originalLocation = location;

            // Only 13 bit address
            location = location & 0x1FFF;
            
            // TIA
            if (location < 0x80)
            {
                return vcsTia_->Read(location);
            }
            
            // Working RAM
            else if (location < 0x100)
            {
                location -= 0x80;
                return ram_->Read(location);
            }
            
            // Riot
            else if (location >= 0x200 && location < 0x300)
            {
                location -= 0x200;
                if (originalLocation == 0x284 && vcsRiot_->Read(location) == 0)
                {
                    int x = 10;
                }
                return vcsRiot_->Read(location);                
            }
            
            // Cartridge ROM
            else if (location >= 0x1000 && location < 0x2000)
            {
                location -= 0x1000;
                return vcsCartridge_->Read(location);
            }    

            throw std::out_of_range(QString("Invalid VCS memory location for read %1").arg(originalLocation).toLocal8Bit().data());
        }

        void VcsMemory::CpuWrite(uint16_t location, uint8_t byte)
        {
            uint16_t originalLocation = location;
            
            // Only 13 bit address
            location = location & 0x1FFF;

            // TIA
            if (location < 0x80)
            {
                vcsTia_->Write(location, byte);
                return;
            }
            
            // Working RAM
            else if (location < 0x100)
            {
                location -= 0x80;
                return ram_->Write(location, byte);
            }
            
            // Riot
            else if (location >= 0x200 && location < 0x300)
            {
                location -= 0x200;
                vcsRiot_->Write(location, byte);
                return;
            }

            // Cartridge ROM
            else if (location >= 0x1000 && location < 0x2000)
            {
                location -= 0x1000;
                vcsCartridge_->Write(location, byte);
                return;
            }  

            throw std::out_of_range(QString("Invalid VCS memory location for write %1").arg(originalLocation).toLocal8Bit().data());
        }

        /*
        void NesMemory::SetLeftController(uint8_t byte)
        {
            leftController_ = byte;
        }

        void NesMemory::SetRightController(uint8_t byte)
        {
            rightController_ = byte;
        }
        */
    }
}

