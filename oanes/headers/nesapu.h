#ifndef _OA_NES_APU_H
#define _OA_NES_APU_H

#include "nesapuchannel.h"
#include "nesmemory.h"

namespace oa
{
    namespace nes
    {
        
        class NesApu
        {
        public:
            NesApu(NesMemory *memory);
            virtual ~NesApu();
            void ExecuteTick();
        private:
            oa::nes::NesMemory *memory_;
            NesApuChannel *channels[4];
        };

    }
}

#endif
