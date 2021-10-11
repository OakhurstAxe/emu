#ifndef _OA_VCS_CARTRIDGEF6_H
#define _OA_VCS_CARTRIDGEF6_H

#include "oaemumemory/headers/memoryram.h"
#include "oaemumemory/headers/memoryrom.h"
#include "vcscartridge.h"

namespace oa
{
    namespace vcs
    {
        
        class VcsCartridgeF6 : public VcsCartridge
        {
        public:
            VcsCartridgeF6();
            
            uint8_t Read(uint16_t location) override;
            void Write(uint16_t location, uint8_t byte) override;
            void LoadData(uint8_t * data, uint32_t size) override;
        private:
            emu::MemoryRom *selectedCartRom_;
            emu::MemoryRom cartRom0_;
            emu::MemoryRom cartRom1_;
            emu::MemoryRom cartRom2_;
            emu::MemoryRom cartRom3_;
        };
        
    }
}

#endif

