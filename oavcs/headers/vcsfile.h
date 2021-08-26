#ifndef _VCS_FILE_H
#define _VCS_FILE_H

#include <stdint.h>
#include <QString>

namespace oa
{
    namespace vcs
    {
        class VcsFile
        {
        public:
            void LoadFile(QString fileName);
            uint8_t *GetProgRomData();
            uint8_t GetProgRomSize();

        private:
            uint16_t progRomSize_;
            uint8_t progRomData_[0x1000];
        };
        
    }
}

#endif


