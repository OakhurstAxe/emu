
#include "headers/vcscartridgef6.h"

namespace oa
{
    namespace vcs
    {
        VcsCartridgeF6::VcsCartridgeF6() :
            cartRom0_(0x1000, "Cartridge ROM Bank 0"),
            cartRom1_(0x1000, "Cartridge ROM Bank 1"),
            cartRom2_(0x1000, "Cartridge ROM Bank 2"),
            cartRom3_(0x1000, "Cartridge ROM Bank 3")
        {
            selectedCartRom_ = &cartRom3_;
        }
        
        uint8_t VcsCartridgeF6::Read(uint16_t location)
        {
            if (location == 0xFF6)
            {
                selectedCartRom_ = &cartRom0_;
            }
            else if (location == 0xFF7)
            {
                selectedCartRom_ = &cartRom1_;
            }
            else if (location == 0xFF8)
            {
                selectedCartRom_ = &cartRom2_;
            }
            else if (location == 0xFF9)
            {
                selectedCartRom_ = &cartRom3_;
            }
            return selectedCartRom_->Read(location);
        }
        
        void VcsCartridgeF6::Write(uint16_t location, uint8_t byte)
        {
            if (location == 0xFF6)
            {
                selectedCartRom_ = &cartRom0_;
                return;
            }
            else if (location == 0xFF7)
            {
                selectedCartRom_ = &cartRom1_;
                return;
            }
            else if (location == 0xFF8)
            {
                selectedCartRom_ = &cartRom2_;
                return;
            }
            else if (location == 0xFF9)
            {
                selectedCartRom_ = &cartRom3_;
                return;
            }

            Q_UNUSED(byte);
            throw std::out_of_range(QString("Cannot write to Cart ROM %1").arg(location).toLocal8Bit().data());
        }
        
        void VcsCartridgeF6::LoadData(uint8_t* data, uint32_t size)
        {
            cartRom0_.LoadData(&data[0x1000 * 0], 0x1000);
            cartRom1_.LoadData(&data[0x1000 * 1], 0x1000);
            cartRom2_.LoadData(&data[0x1000 * 2], 0x1000);
            cartRom3_.LoadData(&data[0x1000 * 3], 0x1000);
        }
        
    }
}




