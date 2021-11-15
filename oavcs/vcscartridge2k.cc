
#include "headers/vcscartridge2k.h"

namespace oa
{
    namespace vcs
    {
        VcsCartridge2K::VcsCartridge2K(VcsParameters *vcsParameters) : VcsCartridge(0x800, "Cartridge Program ROM", vcsParameters)
        {
        }
        
        uint8_t VcsCartridge2K::Read(uint16_t location)
        {
            if (location > 0x800)
            {
                location -= 0x800;
            }
            return VcsCartridge::ReadOffset(location, 0);
        }
        
        void VcsCartridge2K::LoadData(uint8_t* data, uint16_t size)
        {
            VcsCartridge::LoadData(data, size);
        }
        
    }
}

