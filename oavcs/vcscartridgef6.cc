
#include "headers/vcscartridgef6.h"

namespace oa
{
    namespace vcs
    {
        VcsCartridgeF6::VcsCartridgeF6(VcsParameters *vcsParameters) : VcsCartridge(0x4000, "Cartridge ROM Bank", vcsParameters)
        {
            memoryOffset_ = 0x3000;
        }
        
        uint8_t VcsCartridgeF6::Read(uint16_t location)
        {
            SetMemoryOffset(location);

            return VcsCartridge::Read(location + memoryOffset_);
        }
        
        void VcsCartridgeF6::Write(uint16_t location, uint8_t byte)
        {
            if (SetMemoryOffset(location))
            {
                return;
            }

            Q_UNUSED(byte);
            throw std::out_of_range(QString("Cannot write to Cart ROM %1").arg(location).toLocal8Bit().data());
        }
        
        bool VcsCartridgeF6::SetMemoryOffset(uint16_t location)
        {
            if (location == 0xFF6)
            {
                memoryOffset_ = 0x0000;
                return true;
            }
            else if (location == 0xFF7)
            {
                memoryOffset_ = 0x1000;
                return true;
            }
            else if (location == 0xFF8)
            {
                memoryOffset_ = 0x2000;
                return true;
            }
            else if (location == 0xFF9)
            {
                memoryOffset_ = 0x3000;
                return true;
            }
            return false;
        }
    }
}




