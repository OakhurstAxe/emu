#ifndef _OA_VCS_CARTRIDGESC_H
#define _OA_VCS_CARTRIDGESC_H

#include "oaemumemory/headers/memoryram.h"
#include "oaemumemory/headers/memoryrom.h"
#include "vcscartridge.h"

namespace oa
{
    namespace vcs
    {
        
        class VcsCartridgeSC : public VcsCartridge
        {
        public:
            VcsCartridgeSC();
            
            uint8_t Read(uint16_t location) override;
            void Write(uint16_t location, uint8_t byte) override;
            void LoadData(uint8_t * data, uint32_t size) override;
        private:
            // Using RAM because first 128 bytes is writeable
            emu::MemoryRam cartRam_;
        };
        
    }
}

#endif

