#ifndef _OA_VCS_RIOT_H
#define _OA_VCS_RIOT_H

#include "oaemumemory/headers/memoryram.h"

namespace oa
{
    namespace vcs
    {
 
        class VcsRiot : public emu::MemoryRam
        {
        public:
            VcsRiot();
            virtual ~VcsRiot();
            void Reset();
            void ExecuteTick();
            virtual uint8_t Read(uint16_t location) override;
            virtual void Write(uint16_t location, uint8_t byte) override;
        private:
            uint16_t step_;
            uint16_t stepCount_;
            bool underflowFlag1_;
            bool underflowFlag2_;
        };
        
    }
}

#endif
