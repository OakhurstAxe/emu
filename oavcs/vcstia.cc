
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
        }
        
        void VcsTia::ExecuteTick()
        {
            cycle_++;
            if (cycle_ > 228)
            {
                // Set rendering registers for when scrolling happens
                cycle_ = 0;
                scanLine_++;
                if (scanLine_ > 262)
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
            
            if (scanLine_ < 38 || scanLine_ > 230)
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

            screen_[screenY * 160 + screenX] = background;
            if (screenX < 4)
            {
                if ((Read(REG_PF0) & 0x0f) & screenX)
                {
                    screen_[screenY * 160 + screenX]  = playfield;
                }
            }
            else if (screenX < 12)
            {
                if (Read(REG_PF1) & (screenX - 4))
                {
                    screen_[screenY * 160 + screenX]  = playfield;
                }                    
            }
            else if (screenX < 20)
            {
                if (Read(REG_PF1) & (screenX - 12))
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

    }
}

