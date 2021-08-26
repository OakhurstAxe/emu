
#include "headers/vcsriot.h"

namespace oa
{
    namespace vcs
    {
 
        VcsRiot::VcsRiot() : MemoryRam(0x100, "VCS RIOT Registers")
        {
        }
        
        VcsRiot::~VcsRiot()
        {
        }
        
        void VcsRiot::Reset()
        {
            step_ = 1;
            underflowFlag1_ = false;
            underflowFlag2_ = false;
        }
        
        void VcsRiot::ExecuteTick()
        {
            stepCount_++;
            if (stepCount_ < step_)
            {
                return;
            }

            uint8_t timer = MemoryRam::Read(0x84);
            timer--;
            if (timer == 0)
            {
                step_ = 1;
            }
            
            if (timer == 0xFF)
            {
                MemoryRam::Write(0x85, 0xC0);
            }
            MemoryRam::Write(0x84, timer);
        }
        
        uint8_t VcsRiot::Read(uint16_t location)
        {
            if (location == 0x84)
            {
                uint8_t byte = MemoryRam::Read(0x85);
                byte = byte & 0x7F;
                MemoryRam::Write(0x85, byte);
            }
            if (location == 0x85)
            {
                uint8_t byte = MemoryRam::Read(0x85);
                byte = byte & 0xBF;
                MemoryRam::Write(0x85, byte);
            }
            return MemoryRam::Read(location);
        }

        void VcsRiot::Write(uint16_t location, uint8_t byte)
        {
            if (location == 0x94)
            {
                MemoryRam::Write(0x84, byte - 1);
                step_ = 1;
                stepCount_ = 0;
                return;
            }
            if (location == 0x95)
            {
                MemoryRam::Write(0x84, byte - 1);
                step_ = 8;
                stepCount_ = 0;
                return;
            }
            if (location == 0x96)
            {
                MemoryRam::Write(0x84, byte - 1);
                step_ = 64;
                stepCount_ = 0;
                return;
            }
            if (location == 0x97)
            {
                MemoryRam::Write(0x84, byte - 1);
                step_ = 1024;
                stepCount_ = 0;
                return;
            }

            MemoryRam::Write(location, byte);
            
        }
        
    }
}
