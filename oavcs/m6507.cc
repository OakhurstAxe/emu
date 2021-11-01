
#include "headers/m6507.h"

#include <QDebug>

namespace oa
{
    namespace vcs
    {
        
        M6507::M6507(emu::IMemoryMapper *memory) : M6502(memory)
        {
            stackPointerPage_ = 0x00;
        }

    }
}
