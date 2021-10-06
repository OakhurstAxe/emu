#ifndef _OA_VCS_CARTRIDGE4K_H
#define _OA_VCS_CARTRIDGE4K_H

#include "oaemumemory/headers/memoryram.h"
#include "oaemumemory/headers/memoryrom.h"
#include "vcscartridge.h"

namespace oa
{
    namespace vcs
    {
        
        class VcsCartridge4K : public VcsCartridge
        {
        public:
            VcsCartridge4K();
            
            uint8_t Read(uint16_t location) override;
            void Write(uint16_t location, uint8_t byte) override;
            void LoadData(uint8_t * data, uint32_t size) override;
        private:
            emu::MemoryRam cartRom_;
        };
        
    }
}

#endif
