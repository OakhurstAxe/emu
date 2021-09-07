#ifndef _OA_VCS_CARTRIDGE_H
#define _OA_VCS_CARTRIDGE_H

#include "oaemumemory/headers/memoryrom.h"

namespace oa
{
    namespace vcs
    {
        
        class VcsCartridge : public emu::MemoryRom
        {
        public:
            VcsCartridge();
        };
       
    }
}

#endif


