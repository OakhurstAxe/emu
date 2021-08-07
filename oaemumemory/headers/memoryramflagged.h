#ifndef _OA_EMU_MEMORYRAMFLAGGED_H
#define _OA_EMU_MEMORYRAMFLAGGED_H

#include "memoryram.h"

namespace oa
{
    namespace emu
    {

        class MemoryRamFlagged : public MemoryRam
        {
        public:
            MemoryRamFlagged(uint16_t size, QString name);

            virtual uint8_t Read(uint16_t location) override;
            virtual void Write(uint16_t location, uint8_t byte) override;
            bool IsReadFlagSet(uint16_t location);
            bool IsWriteFlagSet(uint16_t location);
        private:
            bool *isReadFlagged_;
            bool *isWriteFlagged_;
        };

    }
}

#endif

