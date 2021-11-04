
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
#define REG_T102T  (0x297 - REG_OFFSET)

namespace oa
{
    namespace vcs
    {
 
        VcsRiot::VcsRiot() : 
            MemoryRam(0x80, "VCS RIOT Registers"),
            m_gamepad_(*QGamepadManager::instance()->connectedGamepads().begin())
        {
            regSwcha_ = 255;
            regSwchb_ = 11;
            regSwcnt_ = 255;
            regInpt0_ = 255;
            regInpt1_ = 255;
            regInpt2_ = 255;
            regInpt3_ = 255;
            regInpt4_ = 255;
            regInpt5_ = 255;

            connect(&m_gamepad_, SIGNAL(buttonXChanged(bool)), this, SLOT(LeftControllerReset(bool)));
            connect(&m_gamepad_, SIGNAL(buttonYChanged(bool)), this, SLOT(LeftControllerSelect(bool)));
            connect(&m_gamepad_, SIGNAL(axisLeftXChanged(double)), this, SLOT(LeftControllerLeftRight(double)));
            connect(&m_gamepad_, SIGNAL(axisLeftYChanged(double)), this, SLOT(LeftControllerUpDown(double)));
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
        
        void VcsRiot::LeftControllerReset(bool value)
        {
            //qDebug() << "A" << value;
            if (value != 0)
            {
                regSwchb_ &= 0xFE;
            }
            else
            {
                regSwchb_ |= 0x01;
            }
        }

        void VcsRiot::LeftControllerSelect(bool value)
        {
            //qDebug() << "A" << value;
            if (value != 0)
            {
                regSwchb_ &= 0xFD;
            }
            else
            {
                regSwchb_ |= 0x02;
            }
        }

        void VcsRiot::LeftControllerUpDown(double value)
        {
            //qDebug() << "Button Up" << value;
            if (value < 0)
            {
                regSwcha_ |= 0x30;
                regSwcha_ &= 0xEF;
            }
            else if (value > 0)
            {
                regSwcha_ |= 0x30;
                regSwcha_ &= 0xDF;
            }
            else if (value == 0)
            {
                regSwcha_ |= 0x30;
            }
        }
            
        void VcsRiot::LeftControllerLeftRight(double value)
        {
            //qDebug() << "Left X" << value;
            if (value < 0)
            {
                regSwcha_ |= 0xC0;
                regSwcha_ &= 0xBF;
            }
            else if (value > 0)
            {
                regSwcha_ |= 0xC0;
                regSwcha_ &= 0x7F;
            }
            else if (value == 0)
            {
                regSwcha_ |= 0xC0;
            }
        }
        
        uint8_t VcsRiot::Read(uint16_t location)
        {
            location &= 0x7F;

            if (location == REG_SWCHA)
            {
                return regSwcha_;
            }
            else if (location == REG_SWCNT)
            {
                return regInpt4_;
            }
            else if (location == REG_SWCHB)
            {
                return regSwchb_;
            }
            else if (location == REG_SWBCNT)
            {
                return regSwcnt_;
            }
            else if (location == REG_INTIM)
            {
                step_ = prevStep_;
            }
            else if (location == REG_INSTAT)
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
            else if (location == REG_TIM8T)
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
            else if (location == REG_TIM64T)
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
            else if (location == REG_T102T)
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
