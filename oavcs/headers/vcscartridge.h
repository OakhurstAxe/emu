#ifndef _OA_VCS_CARTRIDGE_H
#define _OA_VCS_CARTRIDGE_H

#include "oaemumemory/headers/memoryrom.h"
#include "vcsparameters.h"

namespace oa
{
    namespace vcs
    {

        class VcsCartridge
        {
        public:
            virtual ~VcsCartridge() {};
            virtual uint8_t Read(uint16_t location) = 0;
            virtual void Write(uint16_t location, uint8_t byte) = 0;
            virtual void LoadData(uint8_t* data, uint32_t size) = 0;
            static VcsCartridge* GetCartridge(VcsParameters *vcsParameters);
        };
        
    }
}

#endif


