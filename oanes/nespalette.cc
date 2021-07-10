
#include "headers/nespalette.h"

namespace oa
{
    namespace nes
    {

        const int16_t entries_2C03 [] = {
            333, 14,  6,326,403,503,510,420,320,120, 31, 40, 22,  0,  0,  0,
            555,036,027,407,507,704,700,630,430,140, 40, 53, 44,  0,  0,  0,
            777,357,447,637,707,737,740,750,660,360, 70,276, 77,  0,  0,  0,
            777,567,657,757,747,755,764,772,773,572,473,276,467,  0,  0,  0
        };
        const int16_t entires_RC03B [] = {
            333, 14,  6,326,403,503,510,420,320,100, 31, 40, 22,  0,  0,  0,
            555, 16, 27,407,507,704,700,630,430,140, 40, 53, 44,  0,  0,  0,
            777,357,447,637,707,717,740,750,660,340, 70,276, 77,  0,  0,  0,
            777,547,657,757,747,755,764,772,773,552,473,276,467,  0,  0,  0
        };
        const int16_t entires_RP2C04_0001 [] = {
            755,637,700,447, 44,120,222,704,777,333,750,503,403,660,320,777,
            357,653,310,360,467,657,764,027,760,276,000,200,666,444,707, 14,
              3,567,757, 70, 77, 22, 53,507,  0,420,747,510,407,  6,740,  0,
              0,140,555, 31,572,326,770,630, 20, 36, 40,111,773,737,430,473
        };
        const int16_t entires_RP2C04_0002 [] = {
              0,750,430,572,473,737, 44,567,700,407,773,747,777,637,467,040,
             20,357,510,666, 53,360,200,447,222,707,  3,276,657,320,  0,326,
            403,764,740,757, 36,310,555,  6,507,760,333,120,027,  0,660,777,
            653,111, 70,630, 22, 14,704,140,  0, 77,420,770,755,503, 31,444
        };
        const int16_t entires_RP2C04_0003 [] = {
            507,737,473,555, 40,777,567,120, 14,  0,764,320,704,666,653,467,
            447,044,503, 27,140,430,630,053,333,326,  0,  6,700,510,747,755,
            637, 20,  3,770,111,750,740,777,360,403,357,707, 36,444,  0,310,
             77,200,572,757,420, 70,660,222, 31,  0,657,773,407,276,760, 22
        };
        const int16_t entires_RP2C04_0004 [] = {
            430,326, 44,660,  0,755, 14,630,555,310, 70,  3,764,770, 40,572,
            737,200,027,747,  0,222,510,740,653, 53,447,140,403,  0,473,357,
            503,031,420,  6,407,507,333,704,022,666, 36, 20,111,773,444,707,
            757,777,320,700,760,276,777,467,  0,750,637,567,360,657, 77,120
        };
        
        NesPalette:: NesPalette()
        {
            SetupPalettes();
        }

        void NesPalette::SetPaletteChoice(Palette palette)
        {
            paletteChoice_ = palette;
        }

        QColor NesPalette::GetColor(uint8_t position)
        {
            return GetColor(position, paletteChoice_);
        }

        QColor NesPalette::GetColor(uint8_t position, Palette palette)
        {
            if (position > 64)
            {
                return Qt::white;
            }
            return palette_[palette][position];
        }

        void NesPalette::SetupPalettes()
        {
            SetupPalette(entries_2C03, Palette::Palette_2C03);
            SetupPalette(entires_RC03B, Palette::Palette_RC2C03B);
            SetupPalette(entires_RP2C04_0001, Palette::Palette_RP2C04_0001);
            SetupPalette(entires_RP2C04_0002, Palette::Palette_RP2C04_0002);
            SetupPalette(entires_RP2C04_0003, Palette::Palette_RP2C04_0003);
            SetupPalette(entires_RP2C04_0004, Palette::Palette_RP2C04_0004);
        }

        void NesPalette::SetupPalette(const int16_t entries[], Palette palette)
        {
            for (int index=0; index<64; index++)
            {
                int16_t entry = entries[index];
                int8_t blue = entry % 10;
                int8_t green = (entry % 100 - blue) / 10;
                int8_t red = (entry - green - blue) / 100;
                SetColor(red, green, blue, index, palette);
            }
        }
        
        void NesPalette::SetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t position, Palette palette)
        {
            palette_[static_cast<int>(palette)][position].setRgb(red*32,green*32,blue*32);
        }

    }
}
