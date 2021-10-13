
#include "headers/vcsriot.h"

namespace oa
{
    namespace vcs
    {
 
        VcsRiot::VcsRiot() : MemoryRam(0x100, "VCS RIOT Registers")
        {
        }
        
        void VcsRiot::Reset()
        {
            step_ = 1;
            prevStep_ = 1;
            MemoryRam::Write(0x85, 0);
        }
        
        void VcsRiot::ExecuteTick()
        {
            stepCount_++;
            if (stepCount_ < step_)
            {
                return;
            }

            stepCount_ = 0;
            uint8_t timer = MemoryRam::Read(0x84);
            timer--;
            if (timer == 0xFF)
            {
                step_ = 1;
                MemoryRam::Write(0x85, 0xC0);
            }
            MemoryRam::Write(0x84, timer);
        }
        
        uint8_t VcsRiot::Read(uint16_t location)
        {
            if (location == 0x84)
            {
                step_ = prevStep_;
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
                uint8_t byte = MemoryRam::Read(0x85);
                byte = byte & 0xEF;
                MemoryRam::Write(0x85, byte);

                MemoryRam::Write(0x84, byte - 1);
                step_ = 1;
                prevStep_ = step_;
                stepCount_ = 0;
                return;
            }
            if (location == 0x95)
            {
                uint8_t byte = MemoryRam::Read(0x85);
                byte = byte & 0xEF;
                MemoryRam::Write(0x85, byte);

                MemoryRam::Write(0x84, byte - 1);
                step_ = 8;
                prevStep_ = step_;
                stepCount_ = 0;
                return;
            }
            if (location == 0x96)
            {
                uint8_t byte = MemoryRam::Read(0x85);
                byte = byte & 0xEF;
                MemoryRam::Write(0x85, byte);

                MemoryRam::Write(0x84, byte - 1);
                step_ = 64;
                prevStep_ = step_;
                stepCount_ = 0;
                return;
            }
            if (location == 0x97)
            {
                uint8_t byte = MemoryRam::Read(0x85);
                byte = byte & 0xEF;
                MemoryRam::Write(0x85, byte);

                MemoryRam::Write(0x84, byte - 1);
                step_ = 1024;
                prevStep_ = step_;
                stepCount_ = 0;
                return;
            }
            MemoryRam::Write(location, byte);            
        }
        
    }
}
