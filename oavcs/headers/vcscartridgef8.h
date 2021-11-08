#ifndef _OA_VCS_CARTRIDGEF8_H
#define _OA_VCS_CARTRIDGEF8_H

#include "vcscartridge.h"

namespace oa
{
    namespace vcs
    {
        
        class VcsCartridgeF8 : public VcsCartridge
        {
        public:
            VcsCartridgeF8(VcsParameters *vcsParameters);
            
            uint8_t Read(uint16_t location) override;
            void Write(uint16_t location, uint8_t byte) override;
        private:
            bool SetMemoryOffset(uint16_t location);
            uint16_t memoryOffset_;
        };
        
    }
}

#endif

