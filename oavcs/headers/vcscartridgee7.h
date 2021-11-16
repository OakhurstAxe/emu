#ifndef _OA_VCS_CARTRIDGEE7_H
#define _OA_VCS_CARTRIDGEE7_H

#include "oaemumemory/headers/memoryram.h"
//#include "oaemumemory/headers/memoryrom.h"
#include "vcscartridge.h"

namespace oa
{
    namespace vcs
    {
        
        class VcsCartridgeE7 : public VcsCartridge
        {
        public:
            VcsCartridgeE7(VcsParameters *vcsParameters);
            
            uint8_t Read(uint16_t location) override;
            void Write(uint16_t location, uint8_t byte) override;
        private:
            bool SetMemoryOffset(uint16_t location);            
            uint16_t romMemoryOffset_;
            bool ram1KEnabled_;
            emu::MemoryRam ram1K_;
            emu::MemoryRam ram1KBank_;
            uint16_t ramMemoryOffset_;
        };
        
    }
}

#endif

