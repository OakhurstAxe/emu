
#include "headers/vcscartridgee7.h"

namespace oa
{
    namespace vcs
    {
        VcsCartridgeE7::VcsCartridgeE7(VcsParameters *vcsParameters) : VcsCartridge(0x4000, "Cartridge Program ROM", vcsParameters),
            ram1K_(0x400, "Cartridge RAM 1K"),
            ram1KBank_(0x400, "Cartridge RAM 1K Bank")
        {
            romMemoryOffset_ = 0;
            ramMemoryOffset_ = 0;
            ram1KEnabled_ = false;
        }
        
        uint8_t VcsCartridgeE7::Read(uint16_t location)
        {
            SetMemoryOffset(location);
            
            if (location < 0x800)
            {
                if (ram1KEnabled_)
                {
                    return ram1K_.Read(location);
                } 
                else 
                {
                    return VcsCartridge::ReadOffset(location, romMemoryOffset_);
                }                
            }
            if (location < 0xA00) // Normal bank ram
            {
                location -= 0x800;
                return ram1KBank_.Read(location + ramMemoryOffset_);
            }
            
            // Else static last bank
            location -= 0xA00;
            return VcsCartridge::ReadOffset(location, 0x3A00); // Last 1.5K of ROM
        }
        
        void VcsCartridgeE7::Write(uint16_t location, uint8_t byte)
        {
            if (location < 0x400 && ram1KEnabled_)
            {
                location += 0x400;
                ram1K_.Write(location, byte);
                return;
            }
            else if (location >= 0x800 && location < 0x900)
            {
                location -= 0x800;
                location += 0x100;
                ram1KBank_.Write(location + ramMemoryOffset_, byte);
                return;
            }
            
            if (SetMemoryOffset(location))
            {
                return;
            }
            throw std::out_of_range(QString("Cannot write to Cart ROM %1").arg(location).toLocal8Bit().data());
        }
        
        bool VcsCartridgeE7::SetMemoryOffset(uint16_t location)
        {
            if (location >= 0xFE0 && location <= 0xFE6)
            {
                romMemoryOffset_ = 0x800 * (location - 0xFE0);
                ram1KEnabled_ = false;
                return true;
            }
            else if (location == 0xFE7)
            {
                ram1KEnabled_ = true;
                return true;
            }
            else if (location >= 0xFE8 && location <= 0xFEB)
            {
                ramMemoryOffset_ = 0x200 * (location - 0xFE8);
            }
        }
    }
}



