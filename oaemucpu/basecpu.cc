#include "headers/basecpu.h"

namespace oa
{
    namespace emu
    {
        
        void BaseCpu::Reset()
        {
            overflowTicks_ = 0;
        }

    }
}
