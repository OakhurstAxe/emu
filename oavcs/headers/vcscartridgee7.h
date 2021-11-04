#ifndef _OA_VCS_CARTRIDGEE7_H
#define _OA_VCS_CARTRIDGEE7_H

#include "oaemumemory/headers/memoryram.h"
#include "oaemumemory/headers/memoryrom.h"
#include "vcscartridge.h"

namespace oa
{
    namespace vcs
    {
        
        class VcsCartridgeE7 : public VcsCartridge
        {
        public:
            VcsCartridgeE7();
            
            uint8_t Read(uint16_t location) override;
            void Write(uint16_t location, uint8_t byte) override;
            void LoadData(uint8_t * data, uint16_t size) override;
        private:
            bool ram1KEnabled_ = false;
            emu::MemoryRam *ramBankSelected_;
            emu::MemoryRom *romBankSelected_;
            emu::MemoryRam cartRam1K_;
            emu::MemoryRam cartRam0_;
            emu::MemoryRam cartRam1_;
            emu::MemoryRam cartRam2_;
            emu::MemoryRam cartRam3_;
            emu::MemoryRom cartRom0_;
            emu::MemoryRom cartRom1_;
            emu::MemoryRom cartRom2_;
            emu::MemoryRom cartRom3_;
            emu::MemoryRom cartRom4_;
            emu::MemoryRom cartRom5_;
            emu::MemoryRom cartRom6_;
            emu::MemoryRom cartRom7_;
        };
        
    }
}

#endif

