
#include "headers/r2a03.h"

namespace oa
{
    namespace nes
    {

        R2A03::R2A03()
        {
            overflowTicks_ = 0;
        }       
     
        void R2A03::ExecuteTick()
        {
            if (overflowTicks_ > 0)
            {
                overflowTicks_--;
                return;
            }
            overflowTicks_ = executeTicks(1) - 1;
        }
    }
}
