#ifndef _OA_VCS_PALETTE_H
#define _OA_VCS_PALETTE_H

#include <QColor>

namespace oa
{
    namespace vcs
    {
 
        class VcsPalette
        {
        public:
            VcsPalette();
            enum Palette
            {
                Palette_Ntsc = 0,
            };
            void SetPaletteChoice(Palette palette);
            QColor GetColor(uint8_t position);
            QColor GetColor(uint8_t position, Palette palette);
        private:
            void SetupPalettes();
            void SetupPalette(const uint32_t entries[], Palette palette);
            void SetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t position, Palette palette);
            Palette paletteChoice_ = Palette::Palette_Ntsc;
            QRgb palette_[1][256];
        };
        
    }
}

#endif


