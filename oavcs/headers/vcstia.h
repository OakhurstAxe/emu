#ifndef VCS_TIA_H
#define VCS_TIA_H

#include "../oaemumemory/headers/memoryram.h"

#define RESOLUTION_X 160
#define RESOLUTION_Y 192

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
            virtual uint8_t Read(uint16_t location) override;
            virtual void Write(uint16_t location, uint8_t byte) override;
        private:
            void MoveObject(uint16_t moveRegister, uint16_t *objectCycle);
            void ApplyMovement();
            void ClearMoveRegisters();
            void RenderPixel();
            uint8_t ReverseBits(uint8_t n);
            uint16_t cycle_;
            uint16_t scanLine_;
            uint8_t screen_[RESOLUTION_X * RESOLUTION_Y];
            bool wSyncSet_;
            uint16_t resP0Cycle_;
            uint16_t resP1Cycle_;
            uint16_t resM0Cycle_;
            uint16_t resM1Cycle_;
            uint16_t resBLCycle_;
        };
        
    }
}

#endif
