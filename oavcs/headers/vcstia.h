#ifndef VCS_TIA_H
#define VCS_TIA_H

#include "../oaemumemory/headers/memoryram.h"

namespace oa
{
    namespace vcs
    {
 
        class VcsTia : public emu::MemoryRam
        {
        public:
            VcsTia();
            virtual ~VcsTia();
            void Reset();
            void ExecuteTick();
            bool IsCpuBlocked();
            uint8_t* GetScreen();
            virtual void Write(uint16_t location, uint8_t byte) override;
        private:
            void RenderPixel();
            uint8_t ReverseBits(uint8_t n);
            uint16_t cycle_;
            uint16_t scanLine_;
            uint8_t screen_[30720];      
            bool wBlankSet_;
            bool rSyncSet_;
        };
        
    }
}

#endif
