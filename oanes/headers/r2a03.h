#ifndef _OA_NES_R2A02
#define _OA_NES_R2A02

#include "../oaemucpu/headers/m6502.h"

namespace oa
{
    namespace nes
    {
        
        class R2A03: public emu::M6502
        {
        public:
            R2A03();
            void ExecuteTick();
        private:
            int overflowTicks_;
        };
        
    }
}

#endif

