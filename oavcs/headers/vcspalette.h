#ifndef _OA_VCS_PALETTE_H
#define _OA_VCS_PALETTE_H

#include <QColor>

#include "vcsconsoletype.h"

namespace oa
{
    namespace vcs
    {
 
        class VcsPalette
        {
        public:
            VcsPalette(VcsConsoleType *vcsConsoleType);
            QColor GetColor(uint8_t position);
        private:
            void SetupPalettes();
            void SetupPalette(const uint32_t entries[], uint8_t palette);
            void SetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t position, uint8_t palette);
            VcsConsoleType *vcsConsoleType_;
            QRgb palette_[3][128];
        };
        
    }
}

#endif


