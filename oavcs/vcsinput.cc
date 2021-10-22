
#include "headers/vcsinput.h"



namespace oa
{
    namespace vcs
    {
        
        VcsInput::VcsInput()
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
        }
        
        uint8_t VcsInput::GetSwchaReg()
        {
            return regSwcha_;
        }
        
        uint8_t VcsInput::GetSwcntReg()
        {
            return regSwcnt_;
        }
        
        uint8_t VcsInput::GetSwchbReg()
        {
            return regSwchb_;
        }
        
        uint8_t VcsInput::GetInpt0Reg()
        {
            return regInpt0_;
        }

        uint8_t VcsInput::GetInpt1Reg()
        {
            return regInpt1_;
        }

        uint8_t VcsInput::GetInpt2Reg()
        {
            return regInpt2_;
        }

        uint8_t VcsInput::GetInpt3Reg()
        {
            return regInpt3_;
        }

        uint8_t VcsInput::GetInpt4Reg()
        {
            return regInpt4_;
        }

        uint8_t VcsInput::GetInpt5Reg()
        {
            return regInpt5_;
        }

        void VcsInput::SetInput(int input)
        {
            switch (input)
            {
                case (CONSOLE_RESET):  // swchb bit 0
                {
                    regSwchb_ &= 0xFE;
                    break;
                }
                case (CONSOLE_RESET_OFF):  // swchb bit 0
                {
                    regSwchb_ |= 0x01;
                    break;
                }
                case (CONSOLE_SELECT):  // swchb bit 1
                {
                    regSwchb_ &= 0xFD;
                    break;
                }
                case (CONSOLE_SELECT_OFF):  // swchb bit 1
                {
                    regSwchb_ |= 0x02;
                    break;
                }
                case (CONSOLE_COLOR):  // swchb bit 3
                {
                    regSwchb_ &= 0xFB;
                    break;
                }
                case (CONSOLE_COLOR_OFF):  // swchb bit 3
                {
                    regSwchb_ |= 0x04;
                    break;
                }
                case (CONSOLE_P0DIFFICULTY):  // swchb bit 6
                {
                    regSwchb_ &= 0xBF;
                    break;
                }
                case (CONSOLE_P0DIFFICULTY_OFF):  // swchb bit 6
                {
                    regSwchb_ |= 0x40;
                    break;
                }
                case (CONSOLE_P1DIFFICULTY):  // swchb bit 7
                {
                    regSwchb_ &= 0x7F;
                    break;
                }
                case (CONSOLE_P1DIFFICULTY_OFF):  // swchb bit 7
                {
                    regSwchb_ |= 0x80;
                    break;
                }
                case (JOYSTICK1_UP):  // swcha bit 4
                {
                    regSwcha_ |= 0x30;
                    regSwcha_ &= 0xEF;
                    break;
                }
                case (JOYSTICK1_DOWN):  // swcha bit 5
                {
                    regSwcha_ |= 0x30;
                    regSwcha_ &= 0xDF;
                    break;
                }
                case (JOYSTICK1_VERT_ZERO):  // swcha bit 4/5
                {
                    regSwcha_ |= 0x30;
                    break;
                }
                case (JOYSTICK1_LEFT):  // swcha bit 6
                {
                    regSwcha_ |= 0xC0;
                    regSwcha_ &= 0xBF;
                    break;
                }
                case (JOYSTICK1_RIGHT):  // swcha bit 7
                {
                    regSwcha_ |= 0xC0;
                    regSwcha_ &= 0x7F;
                    break;
                }
                case (JOYSTICK1_HORZ_ZERO):  // swcha bit 6/7
                {
                    regSwcha_ |= 0xC0;
                    break;
                }                
                case (JOYSTICK1_FIRE):  // inpt4 bit 4
                {
                    regInpt4_ &= 0x7F;
                    break;
                }                
                case (JOYSTICK1_FIRE_OFF):  // inpt4 bit 4
                {
                    regInpt4_ |= 0x80;
                    break;
                }                
            }    
        }
    
    }
}
