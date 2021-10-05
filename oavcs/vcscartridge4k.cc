
#include "headers/vcscartridge4k.h"

namespace oa
{
    namespace vcs
    {
        VcsCartridge4K::VcsCartridge4K() :
            cartRom_(0x1000, "Cartridge Program ROM")
        {
        }
        
        uint8_t VcsCartridge4K::Read(uint16_t location)
        {
            return cartRom_.Read(location);
        }
        
        void VcsCartridge4K::Write(uint16_t location, uint8_t byte)
        {
            Q_UNUSED(byte);
            throw std::out_of_range(QString("Cannot write to Cart ROM %1").arg(location).toLocal8Bit().data());
        }
        
        void VcsCartridge4K::LoadData(uint8_t* data, uint32_t size)
        {
            cartRom_.LoadData(data, size);
        }
        
    }
}


