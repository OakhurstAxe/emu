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
            VcsCartridge(uint16_t size, QString name, VcsParameters *vcsParameters);
            virtual ~VcsCartridge() {};
            virtual uint8_t ReadA13(uint16_t location, bool a13Set);
            void Write(uint16_t location, uint8_t byte) override;
            static VcsCartridge* GetCartridge(VcsParameters *vcsParameters);
        protected:
            uint8_t ReadOffset(uint16_t location, uint16_t memoryOffest);
            bool hasSuperChip_;
        };
        
    }
}

#endif
