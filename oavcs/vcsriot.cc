
#include "headers/vcsriot.h"

#define REG_INTIM  (0x284 - 0x280)
#define REG_INSTAT (0x285 - 0x280)
#define REG_TIMI1T (0x294 - 0x280)
#define REG_TIM8T  (0x295 - 0x280)
#define REG_TIM64T (0x296 - 0x280)
#define REG_T102T  (0x297 - 0x280)

namespace oa
{
    namespace vcs
    {
 
        VcsRiot::VcsRiot() : MemoryRam(0x80, "VCS RIOT Registers")
        {
        }
        
        void VcsRiot::Reset()
        {
            step_ = 1;
            prevStep_ = 1;
            MemoryRam::Write(REG_INSTAT, 0);
        }
        
        void VcsRiot::ExecuteTick()
        {
            stepCount_++;
            if (stepCount_ < step_)
            {
                return;
            }

            stepCount_ = 0;
            uint8_t timer = MemoryRam::Read(REG_INTIM);
            timer--;
            if (timer == 0xFF)
            {
                step_ = 1;
                MemoryRam::Write(REG_INSTAT, 0xC0);
            }
            MemoryRam::Write(REG_INTIM, timer);
        }
        
        uint8_t VcsRiot::Read(uint16_t location)
        {
            location &= 0x7F;

            if (location == REG_INTIM)
            {
                step_ = prevStep_;
            }
            if (location == REG_INSTAT)
            {
                uint8_t byte = MemoryRam::Read(REG_INSTAT);
                byte = byte & 0xBF;
                MemoryRam::Write(REG_INSTAT, byte);
            }
            return MemoryRam::Read(location);
        }

        void VcsRiot::Write(uint16_t location, uint8_t byte)
        {
            location &= 0x7F;
            
            if (location == REG_TIMI1T)
            {
                uint8_t byte = MemoryRam::Read(REG_INSTAT);
                byte = byte & 0xEF;
                MemoryRam::Write(REG_INSTAT, byte);

                MemoryRam::Write(REG_INTIM, byte - 1);
                step_ = 1;
                prevStep_ = step_;
                stepCount_ = 0;
                return;
            }
            if (location == REG_TIM8T)
            {
                uint8_t byte = MemoryRam::Read(REG_INSTAT);
                byte = byte & 0xEF;
                MemoryRam::Write(REG_INSTAT, byte);

                MemoryRam::Write(REG_INTIM, byte - 1);
                step_ = 8;
                prevStep_ = step_;
                stepCount_ = 0;
                return;
            }
            if (location == REG_TIM64T)
            {
                uint8_t byte = MemoryRam::Read(REG_INSTAT);
                byte = byte & 0xEF;
                MemoryRam::Write(REG_INSTAT, byte);

                MemoryRam::Write(REG_INTIM, byte - 1);
                step_ = 64;
                prevStep_ = step_;
                stepCount_ = 0;
                return;
            }
            if (location == REG_T102T)
            {
                uint8_t byte = MemoryRam::Read(REG_INSTAT);
                byte = byte & 0xEF;
                MemoryRam::Write(REG_INSTAT, byte);

                MemoryRam::Write(REG_INTIM, byte - 1);
                step_ = 1024;
                prevStep_ = step_;
                stepCount_ = 0;
                return;
            }
            MemoryRam::Write(location, byte);            
        }
        
    }
}
