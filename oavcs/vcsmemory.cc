
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
            
            // TIA - If A12=0, A7=0 Mirroring
            if ((location & 0x1080) == 0)
            {
                location &= 0x7F;
                return vcsTia_->Read(location);
            }
            
            // Working RAM
            else if (location < 0x100)
            {
                location -= 0x80;
                return ram_->Read(location);
            }
            
            // PIA RAM Mirrors - A12=0, A9=0, A7=1  //111* 11*1 *111 1111
            else if ((location & 0x1280) == 0x0080)
            {
                location &= 0x00FF;
                return vcsRiot_->Read(location);                
            }

            // PIA I/O Mirrors - A12=0, A9=1, A7=1  //111* 11*1 *111 1111
            else if ((location & 0x1280) == 0x0280)
            {
                location &= 0x00FF;
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

            // TIA - If A12=0, A7=0 Mirroring
            if ((location & 0x1080) == 0)
            {
                location &= 0x7F;
                return vcsTia_->Write(location, byte);
            }
            
            // Working RAM
            else if (location < 0x100)
            {
                location -= 0x80;
                return ram_->Write(location, byte);
            }
            
            // PIA RAM Mirrors - A12=0, A9=0, A7=1  //111* 11*1 *111 1111
            else if ((location & 0x1280) == 0x0080)
            {
                location &= 0x00FF;
                return vcsRiot_->Write(location, byte);
            }
            
            // PIA I/O Mirrors - A12=0, A9=1, A7=1  //111* 11*1 *111 1111
            else if ((location & 0x1280) == 0x0280)
            {
                location &= 0x00FF;
                return vcsRiot_->Write(location, byte);
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

    }
}

