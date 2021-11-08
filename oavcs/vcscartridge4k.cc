
#include "headers/vcscartridge4k.h"

namespace oa
{
    namespace vcs
    {
        VcsCartridge4K::VcsCartridge4K(VcsParameters *vcsParameters) : VcsCartridge(0x1000, "Cartridge Program ROM", vcsParameters)
        {
        }
    }
}


