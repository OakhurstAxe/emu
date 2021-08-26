#ifndef _OA_VCS_MEMORY_H
#define _OA_VCS_MEMORY_H

#include "../oaemumemory/headers/memoryram.h"
#include "../oaemumemory/headers/memoryramflagged.h"
#include "../oaemumemory/headers/memoryrom.h"
#include "../oaemumemory/headers/memorymapper.h"
#include "../oaemucpu/headers/m6502.h"
#include "headers/vcscartridge.h"
#include "headers/vcstia.h"
#include "headers/vcsriot.h"

namespace oa
{
    namespace vcs
    {
        
        class VcsMemory: public emu::MemoryMapper
        {
        public:
            VcsMemory(VcsTia *vscTia, emu::MemoryRam *ram, VcsRiot *vcsRiot, VcsCartridge *vcsCartridge);
            virtual ~VcsMemory();

            uint8_t CpuRead(uint16_t location) override;
            void CpuWrite(uint16_t location, uint8_t byte) override;

        private:
            VcsTia* vcsTia_;
            emu::MemoryRam* ram_;
            VcsRiot *vcsRiot_;
            VcsCartridge* vcsCartridge_;

            uint8_t leftController_ = 0;
            uint8_t rightController_ = 0;
        };

    }
}
#endif

