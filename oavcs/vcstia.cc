
#include "headers/vcstia.h"

#include <QDebug>

#define REG_VSYNC   0x00
#define REG_VBLANK  0x01
#define REG_WSYNC   0x02
#define REG_RSYNC   0x03
#define REG_NUSIZ0  0x04
#define REG_NUSIZ1  0x05
#define REG_COLUP0  0x06
#define REG_COLUP1  0x07
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

#define REG_AUDC0   0x15
#define REG_AUDC1   0x16
#define REG_AUDF0   0x17
#define REG_AUDF1   0x18
#define REG_AUDV0   0x19
#define REG_AUDV1   0x1A

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

#define CLOSE       14
#define MEDIUM      (CLOSE + 16)
#define WIDE        (CLOSE + 32)

#define SPRITEOFFSET 5

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
            if (cycle_ > 67 + vcsConsoleType_->GetXResolution())
            {
                // Set rendering registers for when scrolling happens
                cycle_ = 0;
                scanLine_++;
            }
            
            if ((scanLine_ > 2 + vcsConsoleType_->GetVBlankLines()) && (scanLine_ <= 2 + vcsConsoleType_->GetVBlankLines() + vcsConsoleType_->GetYResolution()) && (cycle_ > 67))// && (cycle_ <= 69 + vcsConsoleType_->GetXResolution()))
            {
                RenderPixel();
            }
            
            // WSYNC 
            if (cycle_ == 3) // Not sure this should be 8, but works pretty good
            {
                wSyncSet_ = false;
            }
        }
        
        bool VcsTia::Repaint()
        {
            return (cycle_ == 0 && scanLine_ == 3);
        }
        
        bool VcsTia::IsCycleZero()
        {
            return (cycle_ == 0);
        }
                    
        uint8_t* VcsTia::GetScreen()
        {
            return screen_;
        }
        
        void VcsTia::MoveObject(uint8_t move, uint16_t *objectCycle)
        {
            int8_t moveValue = (move & 0x70) >> 4;
            if (move & 0x80)
            {
                // twos compliment
                moveValue = moveValue | 0xF8;
            }
            *objectCycle -= moveValue;
            if (*objectCycle < 68)
            {
                *objectCycle = 225;
            }
            if (*objectCycle > 228)
            {
                *objectCycle = 68 + 3;
            }
        }
        
        void VcsTia::ApplyMovement()
        {
            MoveObject(HMP0, &resP0Cycle_);
            MoveObject(HMP1, &resP1Cycle_);
            MoveObject(HMM0, &resM0Cycle_);
            MoveObject(HMM1, &resM1Cycle_);
            MoveObject(HMBL, &resBLCycle_);
        }
        
        void VcsTia::ClearMoveRegisters()
        {
            HMP0 = 0;
            HMP1 = 0;
            HMM0 = 0;
            HMM1 = 0;
            HMBL = 0;
        }

        int16_t VcsTia::GetPlayerPixel(uint8_t graphicsPlayer, uint8_t playerSize,
            uint8_t reflectPlayer, uint8_t color, uint16_t playerCycle)
        {
            int16_t result = -1;
            
            uint8_t spriteData = graphicsPlayer;
            if (spriteData == 0)
            {
                return result;
            }
            if ((reflectPlayer & 0x08) == 0)
            {
                spriteData = ReverseBits(spriteData);
            }
            
            uint16_t position2Cycle = playerCycle;
            uint16_t position3Cycle = playerCycle;
            uint8_t sizeMultiple = 1;
            uint8_t size = playerSize;
            if ((size & 0x07) == 0)
            {
                sizeMultiple = 1;
            }
            else if ((size & 0x07) == 1)
            {
                position2Cycle = playerCycle + CLOSE;
            }
            else if ((size & 0x07) == 2)
            {
                position2Cycle = playerCycle + MEDIUM;
            }
            else if ((size & 0x07) == 3)
            {
                position2Cycle = playerCycle + CLOSE;
                position3Cycle = playerCycle + MEDIUM;
            }
            else if ((size & 0x07) == 4)
            {
                position2Cycle = playerCycle + WIDE;
            }
            else if ((size & 0x07) == 5)
            {
                sizeMultiple = 2;
            }
            else if ((size & 0x07) == 6)
            {
                position2Cycle = playerCycle + MEDIUM;
                position3Cycle = playerCycle + WIDE;
            }
            else if ((size & 0x07) == 7)
            {
                sizeMultiple = 4;
            }

            uint32_t shift = (cycle_ - playerCycle)/ sizeMultiple;
            if (shift < 8 && (((spriteData >> shift) & 0x01) > 0))
            {
                result = color;
                return result;
            }
            shift = (cycle_ - position2Cycle)/ sizeMultiple;
            if (shift < 8 && (((spriteData >> shift) & 0x01) > 0))
            {
                result = color;
                return result;
            }
            shift = (cycle_ - position3Cycle)/ sizeMultiple;
            if (shift < 8 && (((spriteData >> shift) & 0x01) > 0))
            {
                result = color;
                return result;
            }                    

            return result;
        }
 
        int16_t VcsTia::GetPlayfieldPixel()
        {
            uint16_t screenX = cycle_ - 68;
            uint8_t controlPlayfield = CTRLPF;
            uint8_t playfieldColor = COLUPF;
            uint8_t byte;
            int16_t result = -1;
            
            if ((controlPlayfield & 0x02) > 0)
            {
                if (screenX < 80)
                {
                    playfieldColor = COLUP0;
                }
                else
                {
                    playfieldColor = COLUP1;
                }
            }
            if (screenX < 16)
            {
                byte = ((PF0 >> 4) & 0x0f);
                byte = (byte >> (screenX >> 2)) & 0x01;
                if (byte > 0)
                {
                    result = playfieldColor;
                }
            }
            else if (screenX < 48)
            {
                byte = PF1;
                byte = ReverseBits(byte);
                uint8_t shift = (screenX - 16) >> 2;
                byte = (byte >> shift) & 0x01;
                if (byte > 0)
                {
                    result = playfieldColor;
                }
            }
            else if (screenX < 80)
            {
                byte = PF2;
                uint8_t shift = (screenX - 48) >> 2;
                byte = (byte >> shift) & 0x01;
                if (byte > 0)
                {
                    result = playfieldColor;
                }
            }
            if (screenX >= 80)
            {
                if ((controlPlayfield & 0x01) > 0)
                {
                    if (screenX < 112)
                    {
                        byte = PF2;
                        byte = ReverseBits(byte);
                        uint8_t shift = (screenX - 80) >> 2;
                        byte = (byte >> shift) & 0x01;
                        if (byte > 0)
                        {
                            result  = playfieldColor;
                        }
                    }
                    else if (screenX < 144)
                    {
                        byte = PF1;
                        uint8_t shift = (screenX - 112) >> 2;
                        byte = (byte >> shift) & 0x01;
                        if (byte > 0)
                        {
                            result  = playfieldColor;
                        }
                    }
                    else if (screenX <= vcsConsoleType_->GetXResolution())
                    {
                        byte = ((PF0 >> 4) & 0x0f);
                        byte = ReverseBits(byte) >> 4;
                        uint8_t shift = (screenX - 144) >> 2;
                        byte = (byte >> shift) & 0x01;
                        if (byte > 0)
                        {
                            result  = playfieldColor;
                        }
                    }
                }
                else
                {
                    if (screenX < 96)
                    {
                        byte = ((PF0 >> 4) & 0x0f);
                        uint8_t shift = (screenX - 80) >> 2;
                        byte = (byte >> shift) & 0x01;
                        if (byte > 0)
                        {
                            result  = playfieldColor;
                        }
                    }
                    else if (screenX < 128)
                    {
                        byte = PF1;
                        byte = ReverseBits(byte);
                        uint8_t shift = (screenX - 96) >> 2;
                        byte = (byte >> shift) & 0x01;
                        if (byte > 0)
                        {
                            result  = playfieldColor;
                        }
                    }
                    else if (screenX < vcsConsoleType_->GetXResolution())
                    {
                        byte = PF2;
                        uint8_t shift = (screenX - 128) >> 2;
                        byte = (byte >> shift) & 0x01;
                        if (byte > 0)
                        {
                            result  = playfieldColor;
                        }                
                    }
                }
            }
            
            return result;
        }
        
        int16_t VcsTia::GetMisslePixel(uint8_t enable, uint8_t missleReset, uint8_t missleSize,
            uint8_t missleColor, uint16_t missleCycle)
        {
            int16_t result = -1;
                        
            if ((enable & 0x02) > 0 && (missleReset & 0x01) == 0)
            {
                uint16_t position2Cycle_ = missleCycle;
                uint16_t position3Cycle_ = missleCycle;
                uint8_t size = missleSize;
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
                    result  = missleColor;
                }
                if ((position2Cycle_) <= cycle_ && (position2Cycle_ + size) > cycle_)
                {
                    result  = missleColor;
                }
                if ((position3Cycle_) <= cycle_ && (position3Cycle_ + size) > cycle_)
                {
                    result  = missleColor;
                }
            }
            return result;
        }
        int16_t VcsTia::GetBallPixel()
        {
            int16_t result = -1;
            
            if ((ENABL & 0x02) > 0)
            {
                uint8_t size = CTRLPF;
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
                    result = COLUPF;
                }
            }
            
            return result;
        }

        void VcsTia::RenderPixel()
        {
            uint16_t screenX = cycle_ - 68;
            uint16_t screenY = scanLine_ - (3 + vcsConsoleType_->GetVBlankLines());
            uint8_t background = COLUBK;
            
            // Background
            screen_[screenY * vcsConsoleType_->GetXResolution() + screenX] = background;
            
            // Playfield
            int16_t playfieldPixel = GetPlayfieldPixel();
            if (playfieldPixel >= 0)
            {
                screen_[screenY * vcsConsoleType_->GetXResolution() + screenX]  = (uint8_t)playfieldPixel;
            }
            bool pfAbove = false;
            if ((CTRLPF & 0x04) > 0)
            {
                pfAbove = true;
            }
            
            // Get each pixel for collision detection
            int16_t p0Pixel = GetPlayerPixel(GRP0, NUSIZ0, REFP0, COLUP0, resP0Cycle_);
            int16_t p1Pixel = GetPlayerPixel(GRP1, NUSIZ1, REFP1, COLUP1, resP1Cycle_);
            int16_t m0Pixel = GetMisslePixel(ENAM0, RESMP0, NUSIZ0, COLUP0, resM0Cycle_);
            int16_t m1Pixel = GetMisslePixel(ENAM1, RESMP1, NUSIZ1, COLUP1, resM1Cycle_);
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
            if (location == REG_NUSIZ0)
            {
                NUSIZ0 = byte;
            }
            else if (location == REG_NUSIZ1)
            {
                NUSIZ1 = byte;
            }
            else if (location == REG_COLUP0)
            {
                COLUP0 = byte;
            }
            else if (location == REG_COLUP1)
            {
                COLUP1 = byte;
            }
            else if (location == REG_COLUPF)
            {
                COLUPF = byte;
            }
            else if (location == REG_COLUBK)
            {
                COLUBK = byte;
            }
            else if (location == REG_CTRLPF)
            {
                CTRLPF = byte;
            }
            else if (location == REG_REFP0)
            {
                REFP0 = byte;
            }
            else if (location == REG_REFP1)
            {
                REFP1 = byte;
            }
            else if (location == REG_PF0)
            {
                PF0 = byte;
            }
            else if (location == REG_PF1)
            {
                PF1 = byte;
            }
            else if (location == REG_PF2)
            {
                PF2 = byte;
            }
            else if (location == REG_AUDC0)
            {
                AUDC0 = byte;
            }
            else if (location == REG_AUDC1)
            {
                AUDC1 = byte;
            }
            else if (location == REG_AUDF0)
            {
                AUDF0 = byte;
            }
            else if (location == REG_AUDF1)
            {
                AUDF1 = byte;
            }
            else if (location == REG_AUDV0)
            {
                AUDV0 = byte;
            }
            else if (location == REG_AUDV1)
            {
                AUDV1 = byte;
            }
            else if (location == REG_GRP0)
            {
                GRP0 = byte;
            }
            else if (location == REG_GRP1)
            {
                GRP1 = byte;
            }
            else if (location == REG_ENAM0)
            {
                ENAM0 = byte;
            }
            else if (location == REG_ENAM1)
            {
                ENAM1 = byte;
            }
            else if (location == REG_ENABL)
            {
                ENABL = byte;
            }
            else if (location == REG_HMP0)
            {
                HMP0 = byte;
            }
            else if (location == REG_HMP1)
            {
                HMP1 = byte;
            }
            else if (location == REG_HMM0)
            {
                HMM0 = byte;
            }
            else if (location == REG_HMM1)
            {
                HMM1 = byte;
            }
            else if (location == REG_HMBL)
            {
                HMBL = byte;
            }
            else if (location == REG_VDELP0)
            {
                VDELP0 = byte;
            }
            else if (location == REG_VDELP1)
            {
                VDELP1 = byte;
            }
            else if (location == REG_VDELBL)
            {
                VDELBL = byte;
            }
            else if (location == REG_RESMP0)
            {
                RESMP0 = byte;
            }
            else if (location == REG_RESMP1)
            {
                RESMP1 = byte;
            }

            if (location == REG_VSYNC && (byte & 0x02) > 0)
            {
                cycle_ = 0;
                scanLine_ = 0;
            }
            if (location == REG_VBLANK && (byte & 0x02) == 0)
            {
                cycle_ = 0;
                scanLine_ = 2 + vcsConsoleType_->GetVBlankLines();
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
                resP0Cycle_ = cycle_ + SPRITEOFFSET;
                if (resP0Cycle_ < 68)
                {
                    resP0Cycle_ = 71;
                }
            }
            if (location == REG_RESP1)
            {
                resP1Cycle_ = cycle_ + SPRITEOFFSET;
                if (resP1Cycle_ < 68)
                {
                    resP1Cycle_ = 71;
                }
            }
            if (location == REG_RESM0)
            {
                resM0Cycle_ = cycle_ + SPRITEOFFSET;
                if (resM0Cycle_ < 68)
                {
                    resM0Cycle_ = 71;
                }
            }
            if (location == REG_RESM1)
            {
                resM1Cycle_ = cycle_ + SPRITEOFFSET;
                if (resM1Cycle_ < 68)
                {
                    resM1Cycle_ = 71;
                }
            }
            if (location == REG_RESBL)
            {
                resBLCycle_ = cycle_ + SPRITEOFFSET;
                if (resBLCycle_ < 68)
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

        uint8_t VcsTia::GetAudioC0()
        {
            return AUDC0;
        }
        uint8_t VcsTia::GetAudioC1()
        {
            return AUDC1;
        }
        uint8_t VcsTia::GetAudioF0()
        {
            return AUDF0;
        }
        uint8_t VcsTia::GetAudioF1()
        {
            return AUDF1;
        }
        uint8_t VcsTia::GetAudioV0()
        {
            return AUDV0;
        }
        uint8_t VcsTia::GetAudioV1()
        {
            return AUDV1;
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

