
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
            memory_[REG_INSTAT] = 0;
            memory_[REG_SWCHA] = 255;
            memory_[REG_SWCNT] = 255;
            memory_[REG_SWBCNT] = 255;
            selectPressed_ = false;
            resetPressed_ = false;
        }
        
        void VcsRiot::LeftControllerReset(bool value)
        {
            if (value != 0)
            {
                resetPressed_ = true;
            }
            else
            {
                resetPressed_ = false;
            }
        }

        void VcsRiot::LeftControllerSelect(bool value)
        {
            if (value != 0)
            {
                selectPressed_ = true;
            }
            else
            {
                selectPressed_ = false;
            }
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
            overflowTick_ = false;
            uint8_t timer = memory_[REG_INTIM];
            timer--;
            if (timer == 0xFF)
            {
                uint8_t statusByte = memory_[REG_INSTAT];
                statusByte = statusByte | 0xFF;
                memory_[REG_INSTAT] = statusByte;
                step_ = 1;
                overflowTick_ = true;
            }            
            memory_[REG_INTIM] = timer;
        }

        uint8_t VcsRiot::Read(uint16_t location)
        {
            if (location == REG_INTIM)
            {
                ClearTIMnnTUnderflow();
            }
            else if (location == REG_INSTAT)
            {
                ClearInstatUnderflow();
            }
            if (location == REG_SWCHB)
            {
                uint8_t result = 11;
                if (selectPressed_)
                {
                    result &= 0xFD;
                }
                if (resetPressed_)
                {
                    result &= 0xFE;
                }
                return result;
            }
            return MemoryRam::Read(location);
        }

        void VcsRiot::Write(uint16_t location, uint8_t byte)
        {
            if (location == REG_SWCHB)
            {
                uint8_t result = memory_[REG_SWCHB];
                int x = 10;
            }
            if (location == REG_TIMI1T)
            {
                ClearTIMnnTUnderflow();
                memory_[REG_INTIM] = byte - 1;
                step_ = 1;
                stepCount_ = 0;
            }
            else if (location == REG_TIM8T)
            {
                ClearTIMnnTUnderflow();
                memory_[REG_INTIM] = byte - 1;
                step_ = 8;
                stepCount_ = 0;
            }
            else if (location == REG_TIM64T)
            {
                ClearTIMnnTUnderflow();
                memory_[REG_INTIM] = byte - 1;
                step_ = 64;
                stepCount_ = 0;
            }
            else if (location == REG_T1024T)
            {
                ClearTIMnnTUnderflow();
                memory_[REG_INTIM] = byte - 1;
                step_ = 1024;
                stepCount_ = 0;
            }
            MemoryRam::Write(location, byte);            
        }
        
        void VcsRiot::ClearInstatUnderflow()
        {
            if (!overflowTick_)
            {
                uint8_t statusByte = memory_[REG_INSTAT];
                statusByte = statusByte & 0xBF;
                memory_[REG_INSTAT] = statusByte;
            }
        }
        
        void VcsRiot::ClearTIMnnTUnderflow()
        {
            if (!overflowTick_)
            {
                uint8_t statusByte = memory_[REG_INSTAT];
                statusByte = statusByte & 0x7F;
                memory_[REG_INSTAT] = statusByte;
            }
        }
    }
}
