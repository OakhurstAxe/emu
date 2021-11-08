#ifndef _OA_VCS_CARTRIDGE2K_H
#define _OA_VCS_CARTRIDGE2K_H

#include "vcscartridge.h"

namespace oa
{
    namespace vcs
    {
        
        class VcsCartridge2K : public VcsCartridge
        {
        public:
            VcsCartridge2K(VcsParameters *vcsParameters);
            
            uint8_t Read(uint16_t location) override;
            void LoadData(uint8_t* data, uint16_t size) override;
        };
        
    }
}

#endif


