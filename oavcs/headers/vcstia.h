#ifndef _OA_VCS_TIA_H
#define _OA_VCS_TIA_H

#include <QObject>
#include <QtGamepad/QGamepad>

#include "oaemumemory/headers/memoryram.h"
#include "vcsconsoletype.h"
#include "vcspalette.h"

namespace oa
{
    namespace vcs
    {
        class VcsTia : public QObject, public emu::MemoryRam
        {
            Q_OBJECT
            
        public:
            VcsTia(VcsConsoleType *vcsConsoleType);
            virtual ~VcsTia();
            void Reset();
            void ExecuteTick();
            bool IsCpuBlocked();
            QRgb* GetScreen();
            virtual uint8_t Read(uint16_t location) override;
            virtual void Write(uint16_t location, uint8_t byte) override;
            bool Repaint();
            bool IsCycleZero();
            
            uint8_t GetAudioC0();
            uint8_t GetAudioC1();
            uint8_t GetAudioF0();
            uint8_t GetAudioF1();
            uint8_t GetAudioV0();
            uint8_t GetAudioV1();
        public slots:            
            void LeftControllerA(bool value);
        private:
            uint8_t NUSIZ0;
            uint8_t NUSIZ1;
            uint8_t COLUP0;
            uint8_t COLUP1;
            uint8_t COLUPF;
            uint8_t COLUBK;
            uint8_t CTRLPF;
            uint8_t REFP0;
            uint8_t REFP1;
            uint8_t PF0;
            uint8_t PF1;
            uint8_t PF2;
            uint8_t AUDC0 = 0;
            uint8_t AUDC1 = 0;
            uint8_t AUDF0 = 0;
            uint8_t AUDF1 = 0;
            uint8_t AUDV0 = 0;
            uint8_t AUDV1 = 0;
            uint8_t GRP0 = 0;
            uint8_t GRP0DELAY = 0;
            uint8_t GRP1 = 0;
            uint8_t GRP1DELAY = 0;
            uint8_t ENAM0 = 0;
            uint8_t ENAM1 = 0;
            uint8_t ENABL = 0;
            uint8_t ENABLDELAY = 0;
            uint8_t HMP0 = 0;
            uint8_t HMP1 = 0;
            uint8_t HMM0 = 0;
            uint8_t HMM1 = 0;
            uint8_t HMBL = 0;
            uint8_t VDELP0 = 0;
            uint8_t VDELP1 = 0;
            uint8_t VDELBL = 0;
            uint8_t RESMP0 = 0;
            uint8_t RESMP1 = 0;
            uint8_t VSYNC = 2;
            uint8_t VBLANK = 2;
            
            int16_t GetPlayerPixel(uint8_t graphicsPlayer, uint8_t playerSize,
                uint8_t reflectPlayer, uint8_t color, uint16_t playerCycle);
            int16_t GetPlayfieldPixel();
            int16_t GetMisslePixel(uint8_t enable, uint8_t missleReset, uint8_t missleSize,
                uint8_t missleColor, uint16_t missleCycle);
            int16_t GetBallPixel();
            void CheckCollisions(int16_t playfieldPixel,
                int16_t p0Pixel, int16_t p1Pixel,
                int16_t m0Pixel, int16_t m1Pixel,
                int16_t ballPixel);
            void MoveObject(uint8_t move, uint16_t *objectCycle);
            void ApplyMovement();
            void ClearMoveRegisters();
            void RenderPixel();
            uint8_t ReverseBits(uint8_t n);
            VcsConsoleType *vcsConsoleType_;
            uint16_t cycle_;
            uint16_t scanLine_;
            QRgb *screen_;
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
            VcsPalette vcsPalette_;
            
            QGamepad m_gamepad_;
            uint8_t regInpt0_;
            uint8_t regInpt1_;
            uint8_t regInpt2_;
            uint8_t regInpt3_;
            uint8_t regInpt4_;
        };
        
    }
}

#endif
