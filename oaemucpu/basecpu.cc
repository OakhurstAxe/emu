#include "headers/basecpu.h"

namespace oa
{
    namespace emu
    {
        
        void BaseCpu::Reset()
        {
            overflowTicks_ = 0;
        }

        bool BaseCpu::IsOverflowed()
        {
            if (overflowTicks_ > 0)
            {
                return true;
            }
            return false;
        }

    }
}
