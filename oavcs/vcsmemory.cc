
#include "headers/vcsmemory.h"

namespace oa
{
    namespace vcs
    {
        
        VcsMemory::VcsMemory(VcsTia *vcsTia, VcsRiot *vcsRiot, VcsParameters* vcsParameters)
        {
            vcsTia_ = vcsTia;
            vcsRiot_ = vcsRiot;
            vcsCartridge_ = VcsCartridge::GetCartridge(vcsParameters);
        }

        VcsMemory::~VcsMemory()
        {
            delete vcsCartridge_;
        }

        uint8_t VcsMemory::CpuRead(uint16_t location)
        {
            // trick for the EF cartridge mapper
            bool a13Set = (0x2000 & location) > 0;
            uint16_t originalLocation = location;

            // Only 13 bit address
            location = location & 0x1FFF;

            // TIA - If A12=0, A7=0 Mirroring 0 **** 0*** ****
            if ((location & 0x1080) == 0)
            {
                location &= 0x0F;
                location += 0x30;
                return vcsTia_->Read(location);
            }
            
            // Working RAM A12=0, A9=0, A7=1 0 **0* 1*** ****
            else if ((location & 0x1280) == 0x0080)
            {
                location &= 0x7F;
                return vcsRiot_->ReadRam(location);
            }
            
            // PIA I/O Mirrors - A12=0, A9=1, A7=1  0 **1* 1*** ****
            else if ((location & 0x1280) == 0x0280)
            {
                // copies of PIA
                location &= 0x1F;
                
                // Timer locations duplicated in each block
                if (location == 0x06 || location == 0x07)
                {
                    location -= 2;
                }
                return vcsRiot_->Read(location);
            }
            
            // Cartridge ROM
            else if (location >= 0x1000 && location < 0x2000)
            {
                location -= 0x1000;
                return vcsCartridge_->ReadA13(location, a13Set);
            }    

            throw std::out_of_range(QString("Invalid VCS memory location for read %1").arg(originalLocation).toLocal8Bit().data());
        }

        void VcsMemory::CpuWrite(uint16_t location, uint8_t byte)
        {
            uint16_t originalLocation = location;

            // Only 13 bit address
            location = location & 0x1FFF;

            // TIA - If A12=0, A7=0 Mirroring 0 **** 0*** ****
            if ((location & 0x1080) == 0)
            {
                location &= 0xFF;
                if (location >= 0x40)
                {
                    location -= 0x40;
                }
                return vcsTia_->Write(location, byte);
            }
            
            // Working RAM A12=0, A9=0, A7=1 0 **0* 1*** ****
            else if ((location & 0x1280) == 0x0080)
            {
                location &= 0x7F;
                return vcsRiot_->WriteRam(location, byte);
            }
            
            // PIA I/O Mirrors - A12=0, A9=1, A7=1  0 **1* 1*** ****
            else if ((location & 0x1280) == 0x0280)
            {
                // copies of PIA
                location &= 0x7F;
                
                // Timer locations duplicated in each block
                if (location == 0x06 || location == 0x07)
                {
                    location -= 2;
                }
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

