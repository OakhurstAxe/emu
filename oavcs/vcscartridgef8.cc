
#include "headers/vcscartridgef8.h"

namespace oa
{
    namespace vcs
    {
        VcsCartridgeF8::VcsCartridgeF8() :
            cartRom0_(0x1000, "Cartridge ROM Bank 0"),
            cartRom1_(0x1000, "Cartridge ROM Bank 1")
        {
            selectedCartRom_ = &cartRom1_;
        }
        
        uint8_t VcsCartridgeF8::Read(uint16_t location)
        {
            if (location == 0xFF8)
            {
                selectedCartRom_ = &cartRom0_;
            }
            else if (location == 0xFF9)
            {
                selectedCartRom_ = &cartRom1_;
            }
            return selectedCartRom_->Read(location);
        }
        
        void VcsCartridgeF8::Write(uint16_t location, uint8_t byte)
        {
            if (location == 0xFF8)
            {
                selectedCartRom_ = &cartRom0_;
                return;
            }
            else if (location == 0xFF9)
            {
                selectedCartRom_ = &cartRom1_;
                return;
            }

            Q_UNUSED(byte);
            throw std::out_of_range(QString("Cannot write to Cart ROM %1").arg(location).toLocal8Bit().data());
        }
        
        void VcsCartridgeF8::LoadData(uint8_t* data, uint32_t size)
        {
            cartRom0_.LoadData(&data[0x1000 * 0], 0x1000);
            cartRom1_.LoadData(&data[0x1000 * 1], 0x1000);
        }
        
    }
}





