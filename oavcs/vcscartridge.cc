
#include "headers/vcscartridge.h"

#include "headers/vcscartridge2k.h"
#include "headers/vcscartridge4k.h"
#include "headers/vcscartridgef6.h"
#include "headers/vcscartridgee7.h"
#include "headers/vcscartridgesc.h"

namespace oa
{
    namespace vcs
    {

        VcsCartridge* VcsCartridge::GetCartridge(VcsParameters *vcsParameters)
        {
            VcsCartridge *cart;
            
            if (vcsParameters->GetMapper() == "2K")
            {
                cart = new VcsCartridge2K();
            }
            else if (vcsParameters->GetMapper() == "4K")
            {
                cart = new VcsCartridge4K();
            }
            else if (vcsParameters->GetMapper() == "E7")
            {
                cart = new VcsCartridgeE7();
            }
            else if (vcsParameters->GetMapper() == "F6")
            {
                cart = new VcsCartridgeF6();
            }
            else if (vcsParameters->GetMapper() == "SC")
            {
                cart = new VcsCartridgeSC();
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

