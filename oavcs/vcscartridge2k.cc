
#include "headers/vcscartridge2k.h"

namespace oa
{
    namespace vcs
    {
        VcsCartridge2K::VcsCartridge2K() :
            cartRom_(0x800, "Cartridge Program ROM")
        {
        }
        
        uint8_t VcsCartridge2K::Read(uint16_t location)
        {
            if (location > 0x800)
            {
                location -= 0x800;
            }

            return cartRom_.Read(location);
        }
        
        void VcsCartridge2K::Write(uint16_t location, uint8_t byte)
        {
            Q_UNUSED(byte);
            throw std::out_of_range(QString("Cannot write to Cart ROM %1").arg(location).toLocal8Bit().data());
        }
        
        void VcsCartridge2K::LoadData(uint8_t* data, uint32_t size)
        {
            cartRom_.LoadData(data, size);
        }
        
    }
}

