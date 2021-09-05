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
            int16_t GetPlayerPixel(uint8_t graphicsPlayerReg, uint8_t playerSizeReg, 
                uint8_t reflectPlayerReg, uint8_t colorReg, uint16_t playerCycle);
            int16_t GetPlayfieldPixel();
            int16_t GetMisslePixel(uint8_t enableReg, uint8_t missleResetReg, uint8_t missleSizeReg,
                uint8_t missleColorReg, uint16_t missleCycle);
            int16_t GetBallPixel();
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
            bool vertDelayGrP0_ = false;
            uint8_t delayedGrP0_ = 0;
            bool vertDelayGrP1_ = false;
            uint8_t delayedGrP1_ = 0;
            bool vertDelayGrBl_ = false;
            uint8_t delayedGrBl_ = 0;
        };
        
    }
}

#endif
