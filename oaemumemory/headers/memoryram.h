#ifndef _OA_EMU_MEMORYRAM_H
#define _OA_EMU_MEMORYRAM_H

#include "basememory.h"

namespace oa
{
    namespace emu
    {

        class MemoryRam : public BaseMemory
        {
        public:
            MemoryRam(uint16_t size, QString name);
        };

    }
}

#endif

