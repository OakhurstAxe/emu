#ifndef _OA_NES_APU_H
#define _OA_NES_APU_H

#include "nesapuchannel.h"
#include "nesmemory.h"

namespace oa
{
    namespace nes
    {
        
        class Apu
        {
        public:
            Apu(NesMemory *memory);
            virtual ~Apu();
            void ExecuteTick();
        private:
            oa::nes::NesMemory *memory_;
            NesApuChannel *channels[2];
        };

    }
}

#endif
