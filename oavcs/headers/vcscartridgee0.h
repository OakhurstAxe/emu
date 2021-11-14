#ifndef _OA_VCS_CARTRIDGEE0_H
#define _OA_VCS_CARTRIDGEE0_H

#include "vcscartridge.h"

namespace oa
{
    namespace vcs
    {
        
        class VcsCartridgeE0 : public VcsCartridge
        {
        public:
            VcsCartridgeE0(VcsParameters *vcsParameters);
            
            uint8_t Read(uint16_t location) override;
            void Write(uint16_t location, uint8_t byte) override;
        private:
            bool SetMemoryOffset(uint16_t location);
            uint16_t memoryOffset0_;
            uint16_t memoryOffset1_;
            uint16_t memoryOffset2_;
        };
        
    }
}

#endif


