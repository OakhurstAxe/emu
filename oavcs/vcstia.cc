
#include "headers/vcstia.h"

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

#define REG_HMP0    0x20
#define REG_HMP1    0x21
#define REG_HMM0    0x22
#define REG_HMM1    0x23
#define REG_HMBL    0x24
#define REG_HMOVE   0x2A
#define REG_HMCLR   0x2B

#define REG_CXM0P     0x30
#define REG_CXM1P     0x31
#define REG_CXP0FB    0x32
#define REG_CXP1FB    0x33
#define REG_CXM0FB    0x34
#define REG_CXM1FB    0x35
#define REG_CXBLPF    0x36
#define REG_CXPPMM    0x37
#define REG_INPT0     0x38
#define REG_INPT1     0x39
#define REG_INPT2     0x3A
#define REG_INPT3     0x3B
#define REG_INPT4     0x3C
#define REG_INPT5     0x3D

#define REG_COLUBK  0x09

namespace oa
{
    namespace vcs
    {
 
        VcsTia::VcsTia() : MemoryRam(0x7F, "TIA Registers")
        {
            Reset();
        }
        
        VcsTia::~VcsTia()
        {
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
            if (cycle_ > 228)
            {
                // Set rendering registers for when scrolling happens
                cycle_ = 1;
                scanLine_++;
                if (scanLine_ > 262)
                {
                    scanLine_ = 1;
                }
            }
            
            if ((scanLine_ > 40) && (scanLine_ < 40 + RESOLUTION_Y) && (cycle_ > 68))
            {
                RenderPixel();
            }
            
            // WSYNC
            if (cycle_ == 1)
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
            uint8_t moveValue = (move & 0x70) >> 4;
            if (move & 0x80)
            {
                moveValue = -moveValue;
            }
            *objectCycle += moveValue;
            if (*objectCycle < 68)
            {
                *objectCycle = 228;
            }
            if (*objectCycle > 228)
            {
                *objectCycle = 68 + 3;
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
        
        void VcsTia::RenderPixel()
        {
            uint16_t screenX = cycle_ - 69;
            uint16_t screenY = scanLine_ - 41;
            uint8_t background = Read(REG_COLUBK);
            uint8_t playfieldColor = Read(REG_COLUPF);
            uint8_t controlPlayfield = Read(REG_CTRLPF);
            uint8_t byte;
            
            // Background
            screen_[screenY * RESOLUTION_X + screenX] = background;
            
            // Playfield
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
                    screen_[screenY * 160 + screenX]  = playfieldColor;
                }
            }
            else if (screenX < 48)
            {
                byte = Read(REG_PF1);
                byte = (byte >> ((screenX - 16) >> 2)) & 0x01;
                if (byte > 0)
                {
                    screen_[screenY * 160 + screenX]  = playfieldColor;
                }                    
            }
            else if (screenX < 80)
            {
                byte = Read(REG_PF2);
                byte = (byte >> ((screenX - 48) >> 2)) & 0x01;
                if (byte > 0)
                {
                    screen_[screenY * 160 + screenX]  = playfieldColor;
                }                    
            }
            else if (screenX < 112)
            {
                byte = Read(REG_PF2);
                if ((controlPlayfield & 0x01) > 0)
                {
                    byte = ReverseBits(byte);
                }
                byte = (byte >> ((screenX - 80) >> 2)) & 0x01;
                if (byte > 0)
                {
                    screen_[screenY * 160 + screenX]  = playfieldColor;
                }                    
            }
            else if (screenX < 144)
            {
                byte = Read(REG_PF1);
                if ((controlPlayfield & 0x01) > 0)
                {
                    byte = ReverseBits(byte);
                }
                byte = (byte >> ((screenX - 112) >> 2)) & 0x01;
                if (byte > 0)
                {
                    screen_[screenY * 160 + screenX]  = playfieldColor;
                }                    
            }
            else if (screenX < 160)
            {
                byte = Read(REG_PF0);
                if ((controlPlayfield & 0x01) > 0)
                {
                    byte = ReverseBits(byte);
                }
                byte = (byte >> ((screenX - 144) >> 2)) & 0x01;
                if (byte > 0)
                {
                    screen_[screenY * 160 + screenX]  = playfieldColor;
                }
            }
            
            // Sprites
            if ((resP0Cycle_ + 1) <= cycle_ && (resP0Cycle_ + 9) >= cycle_)
            {
                uint8_t spriteData = Read(REG_GRP0);
                uint8_t reflectData = Read(REG_REFP0);
                if (spriteData != 0 && (reflectData & 0x4) > 0)
                {
                    spriteData = ReverseBits(spriteData);
                }
                if ((spriteData >> (cycle_ - resP0Cycle_ - 1) & 0x01) > 0)
                {
                    screen_[screenY * 160 + screenX]  = Read(REG_COLIP0);
                }
            }
            if ((resP1Cycle_ + 1) <= cycle_ && (resP1Cycle_ + 9) >= cycle_)
            {
                uint8_t spriteData = Read(REG_GRP1);
                uint8_t reflectData = Read(REG_REFP1);
                if ((reflectData & 0x4) > 0)
                {
                    spriteData = ReverseBits(spriteData);
                }
                if ((spriteData >> (cycle_ - resP1Cycle_ - 1) & 0x01) > 0)
                {
                    screen_[screenY * 160 + screenX]  = Read(REG_COLIP1);
                }
            }
            
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
        
        uint8_t VcsTia::Read(uint16_t location)
        {
            return MemoryRam::Read(location);
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
                if (resP0Cycle_ < 60)
                {
                    resP0Cycle_ = 63;
                }
            }
            if (location == REG_RESP1)
            {
                resP1Cycle_ = cycle_;
                if (resP1Cycle_ < 60)
                {
                    resP1Cycle_ = 63;
                }
            }
            if (location == REG_RESM0)
            {
                resM0Cycle_ = cycle_;
                if (resM0Cycle_ < 60)
                {
                    resM0Cycle_ = 63;
                }
            }
            if (location == REG_RESM1)
            {
                resM1Cycle_ = cycle_;
                if (resM1Cycle_ < 60)
                {
                    resM1Cycle_ = 63;
                }
            }
            if (location == REG_RESBL)
            {
                resBLCycle_ = cycle_;
                if (resBLCycle_ < 60)
                {
                    resBLCycle_ = 63;
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

