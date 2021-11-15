#ifndef _OA_VCS_CARTRIDGEFE_H
#define _OA_VCS_CARTRIDGEFE_H

#include "vcscartridge.h"

namespace oa
{
    namespace vcs
    {
        
        class VcsCartridgeFE : public VcsCartridge
        {
        public:
            VcsCartridgeFE(VcsParameters *vcsParameters);
            
            uint8_t ReadA13(uint16_t location, bool a13Set) override;
        };
        
    }
}

#endif


