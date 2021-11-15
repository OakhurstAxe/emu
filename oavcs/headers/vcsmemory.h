#ifndef _OA_VCS_MEMORY_H
#define _OA_VCS_MEMORY_H

#include "oaemumemory/headers/imemorymapper.h"
#include "vcscartridge.h"
#include "vcsparameters.h"
#include "vcsriot.h"
#include "vcstia.h"

namespace oa
{
    namespace vcs
    {
        
        class VcsMemory: public emu::IMemoryMapper
        {
        public:
            VcsMemory(VcsTia *vscTia, VcsRiot *vcsRiot, VcsParameters* vcsParameters);
            virtual ~VcsMemory();

            uint8_t CpuRead(uint16_t location) override;
            void CpuWrite(uint16_t location, uint8_t byte) override;

        private:
            VcsTia* vcsTia_;
            VcsRiot *vcsRiot_;
            VcsCartridge* vcsCartridge_;
        };

    }
}
#endif
