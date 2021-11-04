#ifndef _OA_VCS_CARTRIDGE_H
#define _OA_VCS_CARTRIDGE_H

#include <stdint.h>

#include "oaemumemory/headers/memoryrom.h"
#include "vcsparameters.h"

namespace oa
{
    namespace vcs
    {

        class VcsCartridge : public emu::MemoryRom
        {
        public:
            VcsCartridge(uint16_t size, QString name);
            virtual ~VcsCartridge() {};
            static VcsCartridge* GetCartridge(VcsParameters *vcsParameters);
        };
        
    }
}

#endif


