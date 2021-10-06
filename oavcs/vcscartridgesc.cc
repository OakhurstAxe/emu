
#include "headers/vcscartridgesc.h"

namespace oa
{
    namespace vcs
    {
        VcsCartridgeSC::VcsCartridgeSC() :
            cartRam_(0x1000, "Cartridge Program ROM")
        {
        }
        
        uint8_t VcsCartridgeSC::Read(uint16_t location)
        {
            return cartRam_.Read(location);
        }
        
        void VcsCartridgeSC::Write(uint16_t location, uint8_t byte)
        {
            if (location <= 128)
            {
                location += 128;
                cartRam_.Write(location, byte);
                return;
            }
            Q_UNUSED(byte);
            throw std::out_of_range(QString("Cannot write to Cart ROM %1").arg(location).toLocal8Bit().data());
        }
        
        void VcsCartridgeSC::LoadData(uint8_t* data, uint32_t size)
        {
            cartRam_.LoadData(data, size);
        }
        
    }
}



