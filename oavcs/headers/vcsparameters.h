#ifndef _OA_VCS_PARAMETERS_H
#define _OA_VCS_PARAMETERS_H

#include <QString>

#include "vcsconsoletype.h"

namespace oa
{
    namespace vcs
    {
        class VcsParameters
        {
        public:
            VcsParameters();
            ~VcsParameters();
            ConsoleType GetConsoleType();
            void SetConsoleType(ConsoleType consoleType);
            uint8_t* GetCartData();
            void SetCartData(uint8_t* cartData, uint32_t size);
            uint32_t GetCartSize();
            void SetMapper(QString mapper);
            QString GetMapper();
            void SetHasSuperChip(bool hasSuperChip);
            bool GetHasSuperChip();
        private:
            ConsoleType consoleType_;
            uint8_t* cartData_;
            uint32_t cartSize_;
            QString mapper_;
            bool hasSuperChip_;
        };
        
    }
}

#endif
