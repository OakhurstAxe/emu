
#include "headers/vcstia.h"

#include <QDebug>

#define REG_VSYNC   0x00
#define REG_VBLANK  0x01
#define REG_WSYNC   0x02
#define REG_RSYNC   0x03
#define REG_NUSIZ0  0x04
#define REG_NUSIZ1  0x05
#define REG_COLIP0  0x06
#define REG_COLIP1  0x07
#define REG_COLUPF  0x08
#define REG_COLUBK  0x09
#define REG_CTRLPF  0x0A
#define REG_REFP0   0x0B
#define REG_REFP1   0x0C
#define REG_PF0     0x0D
#define REG_PF1     0x0E
#define REG_PF2     0x0F

#define REG_RESP0   0x10
#define REG_RESP1   0x11
#define REG_RESM0   0x12
#define REG_RESM1   0x13
#define REG_RESBL   0x14
#define REG_GRP0    0x1B
#define REG_GRP1    0x1C
#define REG_ENAM0   0x1D
#define REG_ENAM1   0x1E
#define REG_ENABL   0x1F

#define REG_HMP0    0x20
#define REG_HMP1    0x21
#define REG_HMM0    0x22
#define REG_HMM1    0x23
#define REG_HMBL    0x24
#define REG_VDELP0  0x25
#define REG_VDELP1  0x26
#define REG_VDELBL  0x27
#define REG_RESMP0  0x28
#define REG_RESMP1  0x29
#define REG_HMOVE   0x2A
#define REG_HMCLR   0x2B
#define REG_CXCLR   0x2C

#define REG_CXM0P   0x30
#define REG_CXM1P   0x31
#define REG_CXP0FB  0x32
#define REG_CXP1FB  0x33
#define REG_CXM0FB  0x34
#define REG_CXM1FB  0x35
#define REG_CXBLPF  0x36
#define REG_CXPPMM  0x37
#define REG_INPT0   0x38
#define REG_INPT1   0x39
#define REG_INPT2   0x3A
#define REG_INPT3   0x3B
#define REG_INPT4   0x3C
#define REG_INPT5   0x3D

#define CLOSE       16
#define MEDIUM      (CLOSE * 2)
#define WIDE        (CLOSE * 4)

namespace oa
{
    namespace vcs
    {
 
        VcsTia::VcsTia(VcsConsoleType *vcsConsoleType) : MemoryRam(0x7F, "TIA Registers")
        {
            vcsConsoleType_ = vcsConsoleType;
            screen_ = (uint8_t*)malloc(vcsConsoleType_->GetXResolution() * vcsConsoleType_->GetYResolution());
            Reset();
        }
        
        VcsTia::~VcsTia()
        {
            free(screen_);
        }
        
        void VcsTia::Reset()
        {
            cycle_ = 0;
            scanLine_ = 0;
            wSyncSet_ = false;
            resP0Cycle_ = 0;
            resP1Cycle_ = 0;
            
            memset(memory_, 0, 0x7F);
        }
        
        void VcsTia::ExecuteTick()
        {
            cycle_++;
            if (cycle_ > 68 + vcsConsoleType_->GetXResolution())
            {
                // Set rendering registers for when scrolling happens
                cycle_ = 1;
                scanLine_++;
                if (scanLine_ > 40 + vcsConsoleType_->GetYResolution())
                {
                    scanLine_ = 1;
                }
            }
            
            if ((scanLine_ > 40) && (scanLine_ < 40 + vcsConsoleType_->GetYResolution()) && (cycle_ > 68))
            {
                RenderPixel();
            }
            
            // WSYNC 
            if (cycle_ == 6) // Not sure this should be 6, but works pretty good
            {
                wSyncSet_ = false;
            }

        }
        
        uint8_t* VcsTia::GetScreen()
        {
            return screen_;
        }
        
        void VcsTia::MoveObject(uint16_t moveRegister, uint16_t *objectCycle)
        {
            uint8_t move = Read(moveRegister);
            int8_t moveValue = (move & 0x70) >> 4;
            if (move & 0x80)
            {
                // twos compliment
                moveValue = moveValue | 0xF8;
            }
            *objectCycle -= moveValue;
            if (*objectCycle < 69)
            {
                *objectCycle = 228;
            }
            if (*objectCycle > 228)
            {
                *objectCycle = 69 + 3;
            }
        }
        
        void VcsTia::ApplyMovement()
        {
            MoveObject(REG_HMP0, &resP0Cycle_);
            MoveObject(REG_HMP1, &resP1Cycle_);
            MoveObject(REG_HMM0, &resM0Cycle_);
            MoveObject(REG_HMM1, &resM1Cycle_);
            MoveObject(REG_HMBL, &resBLCycle_);
        }
        
        void VcsTia::ClearMoveRegisters()
        {
            Write(REG_HMP0, 0);
            Write(REG_HMP1, 0);
            Write(REG_HMM0, 0);
            Write(REG_HMM1, 0);
            Write(REG_HMBL, 0);
        }
        
        int16_t VcsTia::GetPlayerPixel(uint8_t graphicsPlayerReg, uint8_t playerSizeReg,
            uint8_t reflectPlayerReg, uint8_t colorReg, uint16_t playerCycle)
        {
            int16_t result = -1;
            
            if ((playerCycle) <= cycle_ && (playerCycle + 30) >= cycle_)
            {
                uint8_t spriteData = Read(graphicsPlayerReg);
                uint16_t position2Cycle_ = playerCycle;
                uint16_t position3Cycle_ = playerCycle;
                uint8_t sizeMultiple = 1;
                uint8_t size = Read(playerSizeReg);
                if ((size & 0x07) == 0)
                {
                    sizeMultiple = 1;
                }
                else if ((size & 0x07) == 1)
                {
                    position2Cycle_ = playerCycle + CLOSE;
                }
                else if ((size & 0x07) == 2)
                {
                    position2Cycle_ = playerCycle + MEDIUM;
                }
                else if ((size & 0x07) == 3)
                {
                    position2Cycle_ = playerCycle + CLOSE;
                    position3Cycle_ = playerCycle + MEDIUM;
                }
                else if ((size & 0x07) == 4)
                {
                    position2Cycle_ = playerCycle + WIDE;
                }
                else if ((size & 0x07) == 5)
                {
                    sizeMultiple = 2;
                }
                else if ((size & 0x07) == 6)
                {
                    position2Cycle_ = playerCycle + MEDIUM;
                    position3Cycle_ = playerCycle + WIDE;
                }
                else if ((size & 0x07) == 7)
                {
                    sizeMultiple = 4;
                }
                if (spriteData != 0)
                {
                    if ((Read(reflectPlayerReg) & 0x08) == 0)
                    {
                        spriteData = ReverseBits(spriteData);
                    }
                    if (((spriteData) >> (((cycle_ - playerCycle))/ sizeMultiple) & 0x01) > 0)
                    {
                        result = Read(colorReg);
                    }
                    if (((spriteData) >> (((cycle_ - position2Cycle_))/ sizeMultiple) & 0x01) > 0)
                    {
                        result = Read(colorReg);
                    }
                    if (((spriteData) >> (((cycle_ - position3Cycle_))/ sizeMultiple) & 0x01) > 0)
                    {
                        result = Read(colorReg);
                    }                    
                }
            }
            return result;
        }
 
        int16_t VcsTia::GetPlayfieldPixel()
        {
            uint16_t screenX = cycle_ - 69;
            uint8_t controlPlayfield = Read(REG_CTRLPF);
            uint8_t playfieldColor = Read(REG_COLUPF);
            uint8_t byte;
            int16_t result = -1;
            
            if ((controlPlayfield & 0x02) > 0)
            {
                if (screenX < 80)
                {
                    playfieldColor = Read(REG_COLIP0);
                }
                else
                {
                    playfieldColor = Read(REG_COLIP1);
                }
            }
            if (screenX < 16)
            {
                byte = ((Read(REG_PF0) >> 4) & 0x0f);
                byte = (byte >> (screenX >> 2)) & 0x01;
                if (byte > 0)
                {
                    result  = playfieldColor;
                }
            }
            else if (screenX < 48)
            {
                byte = Read(REG_PF1);
                byte = ReverseBits(byte);
                uint8_t shift = (screenX - 16) >> 2;
                byte = (byte >> shift) & 0x01;
                if (byte > 0)
                {
                    result  = playfieldColor;
                }
            }
            else if (screenX < 80)
            {
                byte = Read(REG_PF2);
                uint8_t shift = (screenX - 48) >> 2;
                byte = (byte >> shift) & 0x01;
                if (byte > 0)
                {
                    result  = playfieldColor;
                }
            }
            else if (screenX < 112)
            {
                byte = Read(REG_PF2);
                if ((controlPlayfield & 0x01) > 0)
                {
                    byte = ReverseBits(byte);
                }
                uint8_t shift = (screenX - 80) >> 2;
                byte = (byte >> shift) & 0x01;
                if (byte > 0)
                {
                    result  = playfieldColor;
                }
            }
            else if (screenX < 144)
            {
                byte = Read(REG_PF1);
                if ((controlPlayfield & 0x01) == 0)
                {
                    byte = ReverseBits(byte);
                }
                uint8_t shift = (screenX - 112) >> 2;
                byte = (byte >> shift) & 0x01;
                if (byte > 0)
                {
                    result  = playfieldColor;
                }
            }
            else if (screenX < vcsConsoleType_->GetXResolution())
            {
                byte = Read(REG_PF0);
                if ((controlPlayfield & 0x01) > 0)
                {
                    byte = ReverseBits(byte);
                }
                uint8_t shift = (screenX - 144) >> 2;
                byte = (byte >> shift) & 0x01;
                if (byte > 0)
                {
                    result  = playfieldColor;
                }
            }
            
            return result;
        }
        
        int16_t VcsTia::GetMisslePixel(uint8_t enableReg, uint8_t missleResetReg, uint8_t missleSizeReg,
            uint8_t missleColorReg, uint16_t missleCycle)
        {
            int16_t result = -1;
                        
            if ((Read(enableReg) & 0x02) > 0 && (Read(missleResetReg) & 0x01) == 0)
            {
                uint16_t position2Cycle_ = missleCycle;
                uint16_t position3Cycle_ = missleCycle;
                uint8_t size = Read(missleSizeReg);
                if ((size & 0x07) == 1)
                {
                    position2Cycle_ = missleCycle + CLOSE;
                }
                else if ((size & 0x07) == 2)
                {
                    position2Cycle_ = missleCycle + MEDIUM;
                }
                else if ((size & 0x07) == 3)
                {
                    position2Cycle_ = missleCycle + CLOSE;
                    position3Cycle_ = missleCycle + MEDIUM;
                }
                else if ((size & 0x07) == 4)
                {
                    position2Cycle_ = missleCycle + WIDE;
                }
                else if ((size & 0x07) == 6)
                {
                    position2Cycle_ = missleCycle + MEDIUM;
                    position3Cycle_ = missleCycle + WIDE;
                }
                size = ((size & 0x30) >> 4);
                switch (size)
                {
                    case (0):
                        size = 1;
                        break;
                    case (1):
                        size = 2;
                        break;
                    case (2):
                        size = 4;
                        break;
                    case (3):
                        size = 8;
                        break;
                }
                if ((missleCycle) <= cycle_ && (missleCycle + size) > cycle_)
                {
                    result  = Read(missleColorReg);
                }
                if ((position2Cycle_) <= cycle_ && (position2Cycle_ + size) > cycle_)
                {
                    result  = Read(missleColorReg);
                }
                if ((position3Cycle_) <= cycle_ && (position3Cycle_ + size) > cycle_)
                {
                    result  = Read(missleColorReg);
                }
            }
            return result;
        }
        int16_t VcsTia::GetBallPixel()
        {
            int16_t result = -1;
            
            if ((Read(REG_ENABL) & 0x02) > 0)
            {
                uint8_t size = Read(REG_CTRLPF);
                size = ((size & 0x30) >> 4);
                switch (size)
                {
                    case (0):
                        size = 1;
                        break;
                    case (1):
                        size = 2;
                        break;
                    case (2):
                        size = 4;
                        break;
                    case (3):
                        size = 8;
                        break;
                }
                if ((resBLCycle_) <= cycle_ && (resBLCycle_ + size) >= cycle_)
                {
                    result  = Read(REG_COLUPF);
                }
            }
            
            return result;
        }

        void VcsTia::RenderPixel()
        {
            uint16_t screenX = cycle_ - 69;
            uint16_t screenY = scanLine_ - 41;
            uint8_t background = Read(REG_COLUBK);
            
            // Background
            screen_[screenY * vcsConsoleType_->GetXResolution() + screenX] = background;
            
            // Playfield
            int16_t playfieldPixel = GetPlayfieldPixel();
            if (playfieldPixel >= 0)
            {
                screen_[screenY * vcsConsoleType_->GetXResolution() + screenX]  = (uint8_t)playfieldPixel;
            }
            bool pfAbove = false;
            if ((Read(REG_CTRLPF) & 0x04) > 0)
            {
                pfAbove = true;
            }
            
            // Get each pixel for collision detection
            int16_t p0Pixel = GetPlayerPixel(REG_GRP0, REG_NUSIZ0, REG_REFP0, REG_COLIP0, resP0Cycle_);
            int16_t p1Pixel = GetPlayerPixel(REG_GRP1, REG_NUSIZ1, REG_REFP1, REG_COLIP1, resP1Cycle_);
            int16_t m0Pixel = GetMisslePixel(REG_ENAM0, REG_RESMP0, REG_NUSIZ0, REG_COLIP0, resM0Cycle_);
            int16_t m1Pixel = GetMisslePixel(REG_ENAM1, REG_RESMP1, REG_NUSIZ1, REG_COLIP1, resM1Cycle_);
            int16_t ballPixel = GetBallPixel();
            
            // Don't display pixel if PF has priority and is set
            if (playfieldPixel == -1 || pfAbove == false)
            {
                // Sprites
                if (p0Pixel >= 0)
                {
                    screen_[screenY * vcsConsoleType_->GetXResolution() + screenX]  = (uint8_t)p0Pixel;
                }
                if (p1Pixel >= 0)
                {
                    screen_[screenY * vcsConsoleType_->GetXResolution() + screenX]  = (uint8_t)p1Pixel;
                }
                
                // Missiles
                if (m0Pixel >= 0)
                {
                    screen_[screenY * vcsConsoleType_->GetXResolution() + screenX]  = (uint8_t)m0Pixel;
                }
                if (m1Pixel >= 0)
                {
                    screen_[screenY * vcsConsoleType_->GetXResolution() + screenX]  = (uint8_t)m1Pixel;
                }

                // Ball
                if (ballPixel >= 0)
                {
                    screen_[screenY * vcsConsoleType_->GetXResolution() + screenX]  = (uint8_t)ballPixel;
                }
            }
            
            CheckCollisions(playfieldPixel,
                p0Pixel, p1Pixel, m0Pixel, m1Pixel,
                ballPixel);
        }
        
        void VcsTia::CheckCollisions(int16_t playfieldPixel,
                int16_t p0Pixel, int16_t p1Pixel,
                int16_t m0Pixel, int16_t m1Pixel,
                int16_t ballPixel)
        {
            // Collisions
            uint8_t collision = MemoryRam::Read(REG_CXM0P);
            if (m0Pixel >= 0 && p1Pixel >= 0)
            {
                collision |= 0x80;
            }
            if (m0Pixel >= 0 && p0Pixel >= 0)
            {
                collision |= 0x40;
            }
            MemoryRam::Write(REG_CXM0P, collision);

            collision = MemoryRam::Read(REG_CXM1P);
            if (m1Pixel >= 0 && p0Pixel >= 0)
            {
                collision |= 0x80;
            }
            if (m1Pixel >= 0 && p1Pixel >= 0)
            {
                collision |= 0x40;
            }
            MemoryRam::Write(REG_CXM1P, collision);

            collision = MemoryRam::Read(REG_CXP0FB);
            if (p0Pixel >= 0 && playfieldPixel >= 0)
            {
                collision |= 0x80;
            }
            if (p0Pixel >= 0 && ballPixel >= 0)
            {
                collision |= 0x40;
            }
            MemoryRam::Write(REG_CXP0FB, collision);

            collision = MemoryRam::Read(REG_CXP1FB);
            if (p1Pixel >= 0 && playfieldPixel >= 0)
            {
                collision |= 0x80;
            }
            if (p1Pixel >= 0 && ballPixel >= 0)
            {
                collision |= 0x40;
            }
            MemoryRam::Write(REG_CXP1FB, collision);
            
            collision = MemoryRam::Read(REG_CXM0FB);
            if (m0Pixel >= 0 && playfieldPixel >= 0)
            {
                collision |= 0x80;
            }
            if (m0Pixel >= 0 && ballPixel >= 0)
            {
                collision |= 0x40;
            }
            MemoryRam::Write(REG_CXM0FB, collision);

            collision = MemoryRam::Read(REG_CXM1FB);
            if (m1Pixel >= 0 && playfieldPixel >= 0)
            {
                collision |= 0x80;
            }
            if (m1Pixel >= 0 && ballPixel >= 0)
            {
                collision |= 0x40;
            }
            MemoryRam::Write(REG_CXM1FB, collision);
            
            collision = MemoryRam::Read(REG_CXBLPF);
            if (ballPixel >= 0 && playfieldPixel >= 0)
            {
                collision |= 0x80;
            }
            MemoryRam::Write(REG_CXBLPF, collision);

            collision = MemoryRam::Read(REG_CXPPMM);
            if (p0Pixel >= 0 && p1Pixel >= 0)
            {
                collision |= 0x80;
            }
            if (m0Pixel >= 0 && m1Pixel >= 0)
            {
                collision |= 0x40;
            }
            MemoryRam::Write(REG_CXPPMM, collision);            
        }
        
        bool VcsTia::IsCpuBlocked()
        {
            bool result = false;
            
            if (wSyncSet_)
            {
                result = true;
            }
            
            return result;
        }
        
        void VcsTia::Write(uint16_t location, uint8_t byte)
        {
            if (location == REG_VBLANK && (byte & 0x02) == 0)
            {
                cycle_ = 0;
                scanLine_ = 40;
            }
            if (location == REG_VSYNC && (byte & 0x02) > 0)
            {
                cycle_ = 0;
                scanLine_ = 0;
            }
            if (location == REG_WSYNC)
            {
                wSyncSet_ = true;
            }

            if (location == REG_RSYNC)
            {
                cycle_ = 0;
            }
            if (location == REG_RESP0)
            {
                resP0Cycle_ = cycle_;
                if (resP0Cycle_ < 69)
                {
                    resP0Cycle_ = 72;
                }
            }
            if (location == REG_RESP1)
            {
                resP1Cycle_ = cycle_;
                if (resP1Cycle_ < 69)
                {
                    resP1Cycle_ = 72;
                }
            }
            if (location == REG_RESM0)
            {
                resM0Cycle_ = cycle_;
                if (resM0Cycle_ < 69)
                {
                    resM0Cycle_ = 71;
                }
            }
            if (location == REG_RESM1)
            {
                resM1Cycle_ = cycle_;
                if (resM1Cycle_ < 69)
                {
                    resM1Cycle_ = 71;
                }
            }
            if (location == REG_RESBL)
            {
                resBLCycle_ = cycle_;
                if (resBLCycle_ < 69)
                {
                    resBLCycle_ = 71;
                }
            }
            if (location == REG_HMOVE)
            {
                ApplyMovement();
            }
            if (location == REG_HMCLR)
            {
                ClearMoveRegisters();
            }
            if (location == REG_RESMP0)
            {
                if ((byte & 0x02) > 0)
                {
                    resM0Cycle_ = resP0Cycle_;
                }
            }
            if (location == REG_RESMP1)
            {
                if ((byte & 0x02) > 0)
                {
                    resM1Cycle_ = resP1Cycle_;
                }
            }
            if (location == REG_CXCLR)
            {
                MemoryRam::Write(REG_CXM0P, 0);
                MemoryRam::Write(REG_CXM1P, 0);
                MemoryRam::Write(REG_CXP0FB, 0);
                MemoryRam::Write(REG_CXP1FB, 0);
                MemoryRam::Write(REG_CXM0FB, 0);
                MemoryRam::Write(REG_CXM1FB, 0);
                MemoryRam::Write(REG_CXBLPF, 0);
                MemoryRam::Write(REG_CXPPMM, 0);
            }
            if (location == REG_VDELP0)
            {
                vertDelayGrP0_ = true;
            }
            if (location == REG_GRP0)
            {
                if (vertDelayGrP0_)
                {
                    delayedGrP0_ = byte;
                    return;
                }                
            }
            if (location == REG_VDELP1)
            {
                vertDelayGrP1_ = true;
            }
            if (location == REG_VDELBL)
            {
                vertDelayGrBl_ = true;                
            }
            if (location == REG_GRP0)
            {
                if (vertDelayGrP1_)
                {
                    vertDelayGrP1_ = false;
                    MemoryRam::Write(REG_GRP1, delayedGrP1_);
                }
            }            
            if (location == REG_GRP1)
            {
                if (vertDelayGrP0_)
                {
                    vertDelayGrP0_ = false;
                    MemoryRam::Write(REG_GRP0, delayedGrP0_);
                }
                if (vertDelayGrBl_)
                {
                    vertDelayGrBl_ = false;
                    MemoryRam::Write(REG_ENABL, delayedGrBl_);
                }                
                if (vertDelayGrP1_)
                {
                    delayedGrP1_ = byte;
                    return;
                } 
            }
            if (location == REG_ENABL)
            {
                if (vertDelayGrBl_)
                {
                    delayedGrBl_ = byte;
                    return;
                } 
            }
            
            MemoryRam::Write(location, byte);
        }

        uint8_t VcsTia::ReverseBits(uint8_t n) 
        {
            uint8_t ans = 0;
            for(int i = 7; i >= 0; i--){
                ans |= (n & 1) <<i;
                n>>=1;
            }
            return ans;
        }
    }
}

