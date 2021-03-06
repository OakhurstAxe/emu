
#include "headers/vcscartridgee0.h"

namespace oa
{
    namespace vcs
    {
        VcsCartridgeE0::VcsCartridgeE0(VcsParameters *vcsParameters) : VcsCartridge(0x2000, "Cartridge ROM Bank", vcsParameters)
        {
            memoryOffset0_ = 0x1000;
            memoryOffset1_ = 0x1000;
            memoryOffset2_ = 0x1000;
        }
        
        uint8_t VcsCartridgeE0::Read(uint16_t location)
        {
            SetMemoryOffset(location);

            if (location < 0x400)
            {
                return VcsCartridge::ReadOffset(location, memoryOffset0_);
            }
            else if (location >= 0x400 && location < 0x800)
            {
                location -= 0x400;
                return VcsCartridge::ReadOffset(location, memoryOffset1_);
            }
            else if (location >= 0x800 && location < 0xC00)
            {
                location -= 0x800;
                return VcsCartridge::ReadOffset(location, memoryOffset2_);
            }
            return VcsCartridge::ReadOffset(location, 0x1000);
        }
        
        void VcsCartridgeE0::Write(uint16_t location, uint8_t byte)
        {
            if (SetMemoryOffset(location))
            {
                return;
            }
            VcsCartridge::Write(location, byte);
        }
        
        bool VcsCartridgeE0::SetMemoryOffset(uint16_t location)
        {
            if (location >= 0xFE0 && location < 0xFE8)
            {
                memoryOffset0_ = 0x400 * (location - 0xFE0);
                return true;
            }
            else if (location >= 0xFE8 && location < 0xFF0)
            {
                memoryOffset1_ = 0x400 * (location - 0xFE8);
                return true;
            }
            else if (location >= 0xFF0 && location < 0xFF8)
            {
                memoryOffset2_ = 0x400 * (location - 0xFF0);
                return true;
            }
            return false;
        }
        
    }
}






