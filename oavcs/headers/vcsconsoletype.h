#ifndef _OA_VCS_CONSOLETYPE_H
#define _OA_VCS_CONSOLETYPE_H

#include <stdint.h>

namespace oa
{
    namespace vcs
    {
 
        enum ConsoleType { NTSC, PAL, SECAM };
        
        class VcsConsoleType
        {
        public:
            VcsConsoleType(ConsoleType consoleType);
            uint8_t GetXResolution();
            uint8_t GetYResolution();
            uint8_t GetFramesPerSecond();
            uint8_t GetVBlankLines();
            uint32_t TicksPerSecond();
            ConsoleType GetVideoType();
        private:
            ConsoleType consoleType_;            
        };
        
    }
}

#endif
