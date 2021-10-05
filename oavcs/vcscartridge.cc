
#include "headers/vcscartridge.h"

#include "headers/vcscartridge2k.h"
#include "headers/vcscartridge4k.h"
#include "headers/vcscartridgesc.h"

namespace oa
{
    namespace vcs
    {

        VcsCartridge* VcsCartridge::GetCartridge(VcsParameters *vcsParameters)
        {
            if (vcsParameters->GetMapper() == "2K")
            {
                VcsCartridge *cart = new VcsCartridge2K();
                cart->LoadData(vcsParameters->GetCartData(), vcsParameters->GetCartSize());
                return cart;
            }
            else if (vcsParameters->GetMapper() == "4K")
            {
                VcsCartridge *cart = new VcsCartridge4K();
                cart->LoadData(vcsParameters->GetCartData(), vcsParameters->GetCartSize());
                return cart;
            }
            throw std::out_of_range(QString("Invalid VCS memory mapper %1").arg(vcsParameters->GetMapper()).toLocal8Bit().data());
        }
    }
}

