#ifndef _OA_EMU_MEMORYMAPPER
#define _OA_EMU_MEMORYMAPPER

#include <stdint.h>

namespace oa
{
    namespace emu
    {
 
        class MemoryMapper
        {
        public:
            virtual uint8_t CpuRead(uint16_t location) = 0;
            virtual void CpuWrite(uint16_t location, uint8_t byte) = 0;
        };
    }
}

#endif
