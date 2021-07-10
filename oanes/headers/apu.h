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
            Apu();
            virtual ~Apu();
            oa::nes::NesMemory *memory;
            void ExecuteTick();
        private:
            int frequency[2];
            EmuApuChannel *channels[2];
        };

    }
}

#endif