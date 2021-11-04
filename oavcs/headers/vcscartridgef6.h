#ifndef _OA_VCS_CARTRIDGEF6_H
#define _OA_VCS_CARTRIDGEF6_H

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
        private:
            bool SetMemoryOffset(uint16_t location);
            uint16_t memoryOffset_;
        };
        
    }
}

#endif

