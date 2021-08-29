#ifndef _OA_VCS_INPUT
#define _OA_VCS_INPUT

#include <stdint.h>

#define CONSOLE_RESET               1
#define CONSOLE_RESET_OFF           2
#define CONSOLE_SELECT              3
#define CONSOLE_SELECT_OFF          4
#define CONSOLE_COLOR               5
#define CONSOLE_COLOR_OFF           6
#define CONSOLE_P0DIFFICULTY        7
#define CONSOLE_P0DIFFICULTY_OFF    8
#define CONSOLE_P1DIFFICULTY        9
#define CONSOLE_P1DIFFICULTY_OFF    10
#define JOYSTICK1_UP                11
#define JOYSTICK1_DOWN              12
#define JOYSTICK1_VERT_ZERO         13
#define JOYSTICK1_LEFT              14
#define JOYSTICK1_RIGHT             15
#define JOYSTICK1_HORZ_ZERO         16
#define JOYSTICK1_FIRE              17
#define JOYSTICK1_FIRE_OFF          18
#define JOYSTICK2_UP                19
#define JOYSTICK2_VERT_ZERO         20
#define JOYSTICK2_DOWN              21
#define JOYSTICK2_LEFT              22
#define JOYSTICK2_RIGHT             23
#define JOYSTICK2_HORZ_ZERO         24
#define JOYSTICK2_FIRE              25
#define JOYSTICK2_FIRE_OFF          26

namespace oa
{
    namespace vcs
    {
        
        class VcsInput
        {
        public:
            VcsInput();
            uint8_t GetSwchaReg();
            uint8_t GetSwcntReg();
            uint8_t GetSwchbReg();
            uint8_t GetInpt0Reg();
            uint8_t GetInpt1Reg();
            uint8_t GetInpt2Reg();
            uint8_t GetInpt3Reg();
            uint8_t GetInpt4Reg();
            uint8_t GetInpt5Reg();
            void SetInput(int input);
        private:
            uint8_t regInpt0_;
            uint8_t regInpt1_;
            uint8_t regInpt2_;
            uint8_t regInpt3_;
            uint8_t regInpt4_;
            uint8_t regInpt5_;
            uint8_t regSwcha_;
            uint8_t regSwchb_;
            uint8_t regSwcnt_;
        };
        
    }
}

#endif
