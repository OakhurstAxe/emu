
#include "headers/vcsriot.h"

#define REG_OFFSET 0x280

#define REG_SWCHA  (0x280 - REG_OFFSET)
#define REG_SWCNT  (0x281 - REG_OFFSET)
#define REG_SWCHB  (0x282 - REG_OFFSET)
#define REG_SWBCNT (0x283 - REG_OFFSET)

#define REG_INTIM  (0x284 - REG_OFFSET)
#define REG_INSTAT (0x285 - REG_OFFSET)
#define REG_TIMI1T (0x294 - REG_OFFSET)
#define REG_TIM8T  (0x295 - REG_OFFSET)
#define REG_TIM64T (0x296 - REG_OFFSET)
#define REG_T1024T (0x297 - REG_OFFSET)

namespace oa
{
    namespace vcs
    {
 
        VcsRiot::VcsRiot() : 
            MemoryRam(0x80, "VCS RIOT Registers"),
            m_gamepad_(*QGamepadManager::instance()->connectedGamepads().begin())
        {
            connect(&m_gamepad_, SIGNAL(buttonXChanged(bool)), this, SLOT(LeftControllerReset(bool)));
            connect(&m_gamepad_, SIGNAL(buttonYChanged(bool)), this, SLOT(LeftControllerSelect(bool)));
            connect(&m_gamepad_, SIGNAL(axisLeftXChanged(double)), this, SLOT(LeftControllerLeftRight(double)));
            connect(&m_gamepad_, SIGNAL(axisLeftYChanged(double)), this, SLOT(LeftControllerUpDown(double)));
        }
        
        void VcsRiot::Reset()
        {
            step_ = 1;
            prevStep_ = 1;
            memory_[REG_INSTAT] = 0;
            memory_[REG_SWCHA] = 255;
            memory_[REG_SWCHB] = 11;
            memory_[REG_SWCNT] = 255;
            memory_[REG_SWBCNT] = 255;            
        }
        
        void VcsRiot::LeftControllerReset(bool value)
        {
            uint8_t byte = memory_[REG_SWCHB];
            if (value != 0)
            {
                byte &= 0xFE;
            }
            else
            {
                byte |= 0x01;
            }
            memory_[REG_SWCHB] = byte;
        }

        void VcsRiot::LeftControllerSelect(bool value)
        {
            uint8_t byte = memory_[REG_SWCHB];
            if (value != 0)
            {
                byte &= 0xFD;
            }
            else
            {
                byte |= 0x02;
            }
            memory_[REG_SWCHB] = byte;
        }

        void VcsRiot::LeftControllerUpDown(double value)
        {
            uint8_t byte = memory_[REG_SWCHA];
            if (value < 0)
            {
                byte |= 0x30;
                byte &= 0xEF;
            }
            else if (value > 0)
            {
                byte |= 0x30;
                byte &= 0xDF;
            }
            else if (value == 0)
            {
                byte |= 0x30;
            }
            memory_[REG_SWCHA] = byte;
        }
            
        void VcsRiot::LeftControllerLeftRight(double value)
        {
            uint8_t byte = memory_[REG_SWCHA];
            if (value < 0)
            {
                byte |= 0xC0;
                byte &= 0xBF;
            }
            else if (value > 0)
            {
                byte |= 0xC0;
                byte &= 0x7F;
            }
            else if (value == 0)
            {
                byte |= 0xC0;
            }
            memory_[REG_SWCHA] = byte;
        }
        
        void VcsRiot::ExecuteTick()
        {
            stepCount_++;
            if (stepCount_ < step_)
            {
                return;
            }

            stepCount_ = 0;
            uint8_t timer = memory_[REG_INTIM];
            timer--;
            if (timer == 0xFF)
            {
                step_ = 1;
                uint8_t statusByte = memory_[REG_INSTAT];
                statusByte = statusByte | 0xC0;
                memory_[REG_INSTAT] = statusByte;
            }
            memory_[REG_INTIM] = timer;
        }

        uint8_t VcsRiot::Read(uint16_t location)
        {
            location &= 0x7F;

            if (location == REG_INTIM)
            {
                if (step_ != prevStep_)
                {
                    memory_[REG_INTIM] = previousTimerStart_;
                }
                step_ = prevStep_;
            }
            else if (location == REG_INSTAT)
            {
                uint8_t result = memory_[REG_INSTAT];
                uint8_t byte = result & 0xBF;
                memory_[REG_INSTAT] = byte;
                return result;
            }
            return MemoryRam::Read(location);
        }

        void VcsRiot::Write(uint16_t location, uint8_t byte)
        {
            location &= 0x7F;
            
            if (location == REG_TIMI1T)
            {
                ClearTIMnnTUnderflow();

                previousTimerStart_ = byte - 1;
                memory_[REG_INTIM] = byte - 1;
                step_ = 1;
                prevStep_ = step_;
                stepCount_ = 0;
                return;
            }
            else if (location == REG_TIM8T)
            {
                ClearTIMnnTUnderflow();

                previousTimerStart_ = byte - 1;
                memory_[REG_INTIM] = byte - 1;
                step_ = 8;
                prevStep_ = step_;
                stepCount_ = 0;
                return;
            }
            else if (location == REG_TIM64T)
            {
                ClearTIMnnTUnderflow();

                previousTimerStart_ = byte - 1;
                memory_[REG_INTIM] = byte - 1;
                step_ = 64;
                prevStep_ = step_;
                stepCount_ = 0;
                return;
            }
            else if (location == REG_T1024T)
            {
                ClearTIMnnTUnderflow();
                
                previousTimerStart_ = byte - 1;
                memory_[REG_INTIM] = byte - 1;
                step_ = 1024;
                prevStep_ = step_;
                stepCount_ = 0;
                return;
            }
            MemoryRam::Write(location, byte);            
        }
        
        void VcsRiot::ClearTIMnnTUnderflow()
        {
            uint8_t statusByte = memory_[REG_INSTAT];
            statusByte = statusByte & 0x7F;
            memory_[REG_INSTAT] = statusByte;
        }
    }
}
