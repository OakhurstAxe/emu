
#include "headers/vcscartridgee7.h"

namespace oa
{
    namespace vcs
    {
        VcsCartridgeE7::VcsCartridgeE7(VcsParameters *vcsParameters) : VcsCartridge(0x1000, "Cartridge Program ROM", vcsParameters),
            cartRam1K_(0x800, "Cartridge RAM Bank 1K"),
            cartRam0_(0xFF, "Cartridge RAM Bank 0"),
            cartRam1_(0xFF, "Cartridge RAM Bank 1"),
            cartRam2_(0xFF, "Cartridge RAM Bank 2"),
            cartRam3_(0xFF, "Cartridge RAM Bank 3"),
            cartRom0_(0x800, "Cartridge RAM Bank 0"),
            cartRom1_(0x800, "Cartridge ROM Bank 1"),
            cartRom2_(0x800, "Cartridge ROM Bank 2"),
            cartRom3_(0x800, "Cartridge ROM Bank 3"),
            cartRom4_(0x800, "Cartridge ROM Bank 4"),
            cartRom5_(0x800, "Cartridge ROM Bank 5"),
            cartRom6_(0x800, "Cartridge ROM Bank 6"),
            cartRom7_(0x800, "Cartridge ROM Bank 7")
        {
            romBankSelected_ = &cartRom0_;
            ramBankSelected_ = &cartRam0_;
        }
        
        uint8_t VcsCartridgeE7::Read(uint16_t location)
        {
            if (location < 0x800)
            {
                if (ram1KEnabled_)
                {
                    return cartRam1K_.Read(location);
                }
                else
                {
                    return romBankSelected_->Read(location);
                }
            }
            else if (location < 0x9FF)
            {
                location -= 0x800;
                return ramBankSelected_->Read(location);
            }
            else
            {
                if (location == 0xFE0)
                {
                    romBankSelected_ = &cartRom0_;
                }
                else if (location == 0xFE1)
                {
                    romBankSelected_ = &cartRom1_;
                }
                else if (location == 0xFE2)
                {
                    romBankSelected_ = &cartRom2_;
                }
                else if (location == 0xFE3)
                {
                    romBankSelected_ = &cartRom3_;
                }
                else if (location == 0xFE4)
                {
                    romBankSelected_ = &cartRom4_;
                }
                else if (location == 0xFE5)
                {
                    romBankSelected_ = &cartRom5_;
                }
                else if (location == 0xFE6)
                {
                    romBankSelected_ = &cartRom6_;
                }
                else if (location == 0xFE7)
                {
                    ram1KEnabled_ = true;
                }
                else if (location == 0xFE8)
                {
                    ramBankSelected_ = &cartRam0_;
                }
                else if (location == 0xFE9)
                {
                    ramBankSelected_ = &cartRam1_;
                }
                else if (location == 0xFEA)
                {
                    ramBankSelected_ = &cartRam2_;
                }
                else if (location == 0xFEB)
                {
                    ramBankSelected_ = &cartRam3_;
                }
                location -= 0x800;
                return cartRom7_.Read(location);
            }                
        }
        
        void VcsCartridgeE7::Write(uint16_t location, uint8_t byte)
        {
            if (location < 0x800)
            {
                if (ram1KEnabled_)
                {
                    location -= 0x400;
                    return cartRam1K_.Write(location, byte);
                }
            }
            else if (location < 0x9FF)
            {
                location -= 0x800;
                return ramBankSelected_->Write(location, byte);
            }

            Q_UNUSED(byte);
            throw std::out_of_range(QString("Cannot write to Cart ROM %1").arg(location).toLocal8Bit().data());
        }
        
        void VcsCartridgeE7::LoadData(uint8_t* data, uint16_t size)
        {
            Q_UNUSED(size);
            
            cartRom0_.LoadData(&data[0x800 * 0], 0x800);
            cartRom1_.LoadData(&data[0x800 * 1], 0x800);
            cartRom2_.LoadData(&data[0x800 * 2], 0x800);
            cartRom3_.LoadData(&data[0x800 * 3], 0x800);
            cartRom4_.LoadData(&data[0x800 * 4], 0x800);
            cartRom5_.LoadData(&data[0x800 * 5], 0x800);
            cartRom6_.LoadData(&data[0x800 * 6], 0x800);
            cartRom7_.LoadData(&data[0x800 * 7], 0x800);
        }
        
    }
}



