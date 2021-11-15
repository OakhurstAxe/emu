
#include "headers/vcscartridgefe.h"

namespace oa
{
    namespace vcs
    {
        VcsCartridgeFE::VcsCartridgeFE(VcsParameters *vcsParameters) : VcsCartridge(0x2000, "Cartridge ROM Bank", vcsParameters)
        {
        }
        
        uint8_t VcsCartridgeFE::ReadA13(uint16_t location, bool a13Set)
        {
            if (a13Set)
            {
                return VcsCartridge::Read(location);
            } else {
                return VcsCartridge::Read(location + 0x1000);
            }
        }
        
    }
}






