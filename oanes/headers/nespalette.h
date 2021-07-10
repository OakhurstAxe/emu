#ifndef _OA_NES_PALETTE_H
#define _OA_NES_PALETTE_H

#include <QColor>

namespace oa
{
    namespace nes
    {
 
        class NesPalette
        {
        public:
            NesPalette();
            enum Palette
            {
                Palette_2C03 = 0,
                Palette_2C05 = 0,
                Palette_RC2C03B = 1,
                Palette_RP2C04_0001 = 2,
                Palette_RP2C04_0002 = 3,
                Palette_RP2C04_0003 = 4,
                Palette_RP2C04_0004 = 5
            };
            void SetPaletteChoice(Palette palette);
            QColor GetColor(uint8_t position);
            QColor GetColor(uint8_t position, Palette palette);
        private:
            void SetupPalettes();
            void SetupPalette(const int16_t entries[], Palette palette);
            void SetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t position, Palette palette);
            Palette paletteChoice_ = Palette::Palette_2C03;
            QColor palette_[6][64];
        };
        
    }
}

#endif

