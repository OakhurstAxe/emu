
#include "headers/m6507.h"

namespace oa
{
    namespace vcs
    {
        
        M6507::M6507(emu::MemoryMapper *memory) : M6502(memory)
        {
            stackPointerMax_ = 0xff;
            stackPointerMin_ = 0x80;
        }
        
        void M6507::ExecuteTick(bool isCpuBlocked)
        {
            if (isCpuBlocked)
            {
                if (overflowTicks_ > 0)
                {
                    overflowTicks_ --;
                }
            }
            else
            {
                emu::M6502::ExecuteTick();
            }
        }
        
    }
}
