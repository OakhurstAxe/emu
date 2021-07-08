#ifndef _OA_EMU_MEMORYROM_H
#define _OA_EMU_MEMORYROM_H

#include <stdint.h>
#include <QString>

#include "basememory.h"

namespace oa
{
    namespace emu
    {
        
        class MemoryRom : public BaseMemory
        {
        public:
            MemoryRom(uint16_t size, QString name);
            void Write(uint16_t location, uint8_t byte) override;
        };

    }
}
#endif
