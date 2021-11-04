
#include "headers/vcscartridgef8.h"

namespace oa
{
    namespace vcs
    {
        VcsCartridgeF8::VcsCartridgeF8() : VcsCartridge(0x2000, "Cartridge ROM Bank")
        {
            memoryOffset_ = 0x1000;
        }
        
        uint8_t VcsCartridgeF8::Read(uint16_t location)
        {
            SetMemoryOffset(location);

            return VcsCartridge::Read(location + memoryOffset_);
        }
        
        void VcsCartridgeF8::Write(uint16_t location, uint8_t byte)
        {
            Q_UNUSED(byte);

            if (SetMemoryOffset(location))
            {
                return;
            }

            throw std::out_of_range(QString("Cannot write to Cart ROM %1").arg(location).toLocal8Bit().data());
        }
        
        bool VcsCartridgeF8::SetMemoryOffset(uint16_t location)
        {
            if (location == 0xFF8)
            {
                memoryOffset_ = 0x0000;
                return true;
            }
            else if (location == 0xFF9)
            {
                memoryOffset_ = 0x1000;
                return true;
            }
            return false;
        }
        
    }
}





