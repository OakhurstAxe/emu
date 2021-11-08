
#include "headers/vcscartridge.h"

#include "headers/vcscartridge2k.h"
#include "headers/vcscartridge4k.h"
#include "headers/vcscartridgef6.h"
#include "headers/vcscartridgef8.h"
#include "headers/vcscartridgee7.h"

namespace oa
{
    namespace vcs
    {

        VcsCartridge::VcsCartridge(uint16_t size, QString name, VcsParameters *vcsParameters) : emu::MemoryRom(size, name)
        {
            hasSuperChip_ = vcsParameters->GetHasSuperChip();
        }
        
        void VcsCartridge::Write(uint16_t location, uint8_t byte)
        {
            if (hasSuperChip_ && location < 0x80)
            {
                memory_[location + 0x80] = byte;
                return;
            }
            MemoryRom::Write(location, byte);            
        }

        VcsCartridge* VcsCartridge::GetCartridge(VcsParameters *vcsParameters)
        {
            VcsCartridge *cart;
            
            if (vcsParameters->GetMapper() == "2K")
            {
                cart = new VcsCartridge2K(vcsParameters);
            }
            else if (vcsParameters->GetMapper() == "4K")
            {
                cart = new VcsCartridge4K(vcsParameters);
            }
            else if (vcsParameters->GetMapper() == "E7")
            {
                cart = new VcsCartridgeE7(vcsParameters);
            }
            else if (vcsParameters->GetMapper() == "F6")
            {
                cart = new VcsCartridgeF6(vcsParameters);
            }
            else if (vcsParameters->GetMapper() == "F8")
            {
                cart = new VcsCartridgeF8(vcsParameters);
            }
            else
            {
                throw std::out_of_range(QString("Invalid VCS memory mapper %1").arg(vcsParameters->GetMapper()).toLocal8Bit().data());
            }

            cart->LoadData(vcsParameters->GetCartData(), vcsParameters->GetCartSize());
            return cart;
        }
    }
}

