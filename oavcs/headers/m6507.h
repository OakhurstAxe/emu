#ifndef _OA_VCS_M6507_H
#define _OA_VCS_M6507_H

#include "oaemucpu/headers/m6502.h"
#include "oaemumemory/headers/imemorymapper.h"

namespace oa
{
    namespace vcs
    {
            
        class M6507: public emu::M6502
        {
        public:
            M6507(emu::IMemoryMapper *memory);
        protected:
        };
        
    }
}

#endif
