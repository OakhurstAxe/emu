
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
            int x = 10;
        }
        
        void VcsTia::Reset()
        {
            cycle_ = 0;
            scanLine_ = 0;
            wBlankSet_ = false;
            rSyncSet_ = false;
            
            memory_[REG_CXM0P] = 0;
            memory_[REG_CXM1P] = 0;
            memory_[REG_CXP0FB] = 0;
            memory_[REG_CXP1FB] = 0;
            memory_[REG_CXM0FB] = 0;
            memory_[REG_CXM1FB] = 0;
            memory_[REG_CXBLPF] = 0;
            memory_[REG_CXPPMM] = 0;
            memory_[REG_INPT0] = 0;
            memory_[REG_INPT1] = 0;
            memory_[REG_INPT2] = 0;
            memory_[REG_INPT3] = 0;
            memory_[REG_INPT4] = 0;
            memory_[REG_CXM0P] = 0;
            memory_[REG_INPT5] = 0;
        }
        
        void VcsTia::ExecuteTick()
        {
            cycle_++;
            if (cycle_ > 227)
            {
                // Set rendering registers for when scrolling happens
                cycle_ = 0;
                scanLine_++;
                if (scanLine_ > 260)
                {
                    scanLine_ = 0;
                }
            }
            
            if (scanLine_ > 40 && scanLine_ < 233 && cycle_ > 68)
            {
                RenderPixel();
            }
            
            if (scanLine_ < 4)
            {
                Write(REG_VSYNC, 2);
            }
            else
            {
                Write(REG_VSYNC, 0);
            }
            
            if (scanLine_ < 40 || scanLine_ > 232)
            {
                Write(REG_VBLANK, 2);
            }
            else
            {
                Write(REG_VBLANK, 0);
            }
            
            if (cycle_ == 0)
            {
                wBlankSet_ = false;
            }

            if (scanLine_ == 0 && cycle_ == 0)
            {
                rSyncSet_ = false;
            }            
        }
        
        uint8_t* VcsTia::GetScreen()
        {
            return screen_;
        }
        
        void VcsTia::RenderPixel()
        {
            uint16_t screenX = cycle_ - 69;
            uint16_t screenY = scanLine_ - 41;
            uint8_t background = Read(REG_COLUBK);
            uint8_t playfield = Read(REG_COLUPF);
            uint8_t byte;
            
            // Background
            screen_[screenY * 160 + screenX] = background;
            
            // Playfield
            if (screenX < 16)
            {
                byte = ((Read(REG_PF0) >> 4) & 0x0f);
                byte = (byte >> (screenX >> 2)) & 0x01;
                if (byte > 0)
                {
                    screen_[screenY * 160 + screenX]  = playfield;
                }
            }
            else if (screenX < 48)
            {
                byte = Read(REG_PF1);
                byte = (byte >> ((screenX - 16) >> 2)) & 0x01;
                if (byte > 0)
                {
                    screen_[screenY * 160 + screenX]  = playfield;
                }                    
            }
            else if (screenX < 80)
            {
                byte = Read(REG_PF2);
                byte = (byte >> ((screenX - 48) >> 2)) & 0x01;
                if (byte > 0)
                {
                    screen_[screenY * 160 + screenX]  = playfield;
                }                    
            }
            else if (screenX < 112)
            {
                byte = Read(REG_PF2);
                byte = ReverseBits(byte);
                byte = (byte >> ((screenX - 80) >> 2)) & 0x01;
                if (byte > 0)
                {
                    screen_[screenY * 160 + screenX]  = playfield;
                }                    
            }
            else if (screenX < 144)
            {
                byte = Read(REG_PF1);
                byte = ReverseBits(byte);
                byte = (byte >> ((screenX - 112) >> 2)) & 0x01;
                if (byte > 0)
                {
                    screen_[screenY * 160 + screenX]  = playfield;
                }                    
            }
            else if (screenX < 160)
            {
                byte = Read(REG_PF0);
                byte = ReverseBits(byte);
                byte = (byte >> ((screenX - 144) >> 2)) & 0x01;
                if (byte > 0)
                {
                    screen_[screenY * 160 + screenX]  = playfield;
                }
            }
        }
        
        bool VcsTia::IsCpuBlocked()
        {
            bool result = false;
            
            if (wBlankSet_ || rSyncSet_)
            {
                result = true;
            }
            
            return result;
        }
        
        void VcsTia::Write(uint16_t location, uint8_t byte)
        {
            if (location == REG_WSYNC)
            {
                wBlankSet_ = true;
            }
            if (location == REG_RSYNC)
            {
                rSyncSet_ = true;
            }
            if (location == REG_PF1)
            {
                int x = 10;
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

