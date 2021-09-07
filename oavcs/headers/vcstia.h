#ifndef _OA_VCS_TIA_H
#define _OA_VCS_TIA_H

#include "oaemumemory/headers/memoryram.h"
#include "vcsconsoletype.h"

namespace oa
{
    namespace vcs
    {
        class VcsTia : public emu::MemoryRam
        {
        public:
            VcsTia(VcsConsoleType *vcsConsoleType);
            virtual ~VcsTia();
            void Reset();
            void ExecuteTick();
            bool IsCpuBlocked();
            uint8_t* GetScreen();
            virtual void Write(uint16_t location, uint8_t byte) override;
        private:
            int16_t GetPlayerPixel(uint8_t graphicsPlayerReg, uint8_t playerSizeReg, 
                uint8_t reflectPlayerReg, uint8_t colorReg, uint16_t playerCycle);
            int16_t GetPlayfieldPixel();
            int16_t GetMisslePixel(uint8_t enableReg, uint8_t missleResetReg, uint8_t missleSizeReg,
                uint8_t missleColorReg, uint16_t missleCycle);
            int16_t GetBallPixel();
            void CheckCollisions(int16_t playfieldPixel,
                int16_t p0Pixel, int16_t p1Pixel,
                int16_t m0Pixel, int16_t m1Pixel,
                int16_t ballPixel);
            void MoveObject(uint16_t moveRegister, uint16_t *objectCycle);
            void ApplyMovement();
            void ClearMoveRegisters();
            void RenderPixel();
            uint8_t ReverseBits(uint8_t n);
            VcsConsoleType *vcsConsoleType_;
            uint16_t cycle_;
            uint16_t scanLine_;
            uint8_t *screen_;
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
