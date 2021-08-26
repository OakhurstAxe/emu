#ifndef VCS_M6507_H
#define VCS_M6507_H

#include "../oaemucpu/headers/m6502.h"
#include "../oaemumemory/headers/memorymapper.h"

namespace oa
{
    namespace vcs
    {
            
        class M6507: public emu::M6502
        {
        public:
            M6507(emu::MemoryMapper *memory);
            virtual void ExecuteTick(bool isCpuBlocked);
        };
        
    }
}

#endif
