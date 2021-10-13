#include "headers/vcsconsoletype.h"

namespace oa
{
    namespace vcs
    {
 
        VcsConsoleType::VcsConsoleType(ConsoleType consoleType)
        {
            consoleType_ = consoleType;
        }
        
        uint8_t VcsConsoleType::GetXResolution()
        {
            return 160;
        }
        
        uint8_t VcsConsoleType::GetYResolution()
        {
            if (consoleType_ == NTSC)
            {
                return 192;
            }
            return 228;
        }
        
        uint8_t VcsConsoleType::GetFramesPerSecond()
        {
            if (consoleType_ == NTSC)
            {
                return 60;
            }
            return 50;
        }
        
        uint8_t VcsConsoleType::GetVBlankLines()
        {
            if (consoleType_ == NTSC)
            {
                return 37;
            }
            return 45;
        }
        
        uint32_t VcsConsoleType::TicksPerSecond()
        {
            if (consoleType_ == NTSC)
            {
                return 3579545;
            }
            return 3546894;
        }
        
        ConsoleType VcsConsoleType::GetVideoType()
        {
            return consoleType_;
        }
    }
}

