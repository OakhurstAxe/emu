#ifndef _OA_VCS_CARTRIDGE2K_H
#define _OA_VCS_CARTRIDGE2K_H

#include "oaemumemory/headers/memoryram.h"
#include "oaemumemory/headers/memoryrom.h"
#include "vcscartridge.h"

namespace oa
{
    namespace vcs
    {
        
        class VcsCartridge2K : public VcsCartridge
        {
        public:
            VcsCartridge2K();
            
            uint8_t Read(uint16_t location) override;
            void Write(uint16_t location, uint8_t byte) override;
            void LoadData(uint8_t * data, uint32_t size) override;
        private:
            emu::MemoryRom cartRom_;
        };
        
    }
}

#endif


