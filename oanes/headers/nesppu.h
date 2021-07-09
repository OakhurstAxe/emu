#ifndef _OA_NES_PPU_H
#define _OA_NES_PPU_H

#include "../oaemucpu/headers/basecpu.h"

#include "../oanes/headers/nesmemory.h"
#include "../oaemucpu/headers/m6502.h"

namespace oa
{
    namespace nes
    {
        
        class NesPpu : emu::BaseCpu
        {
        public:
            oa::nes::NesMemory *memory;
            int executeTicks(int count);
            void reset();
            emu::M6502 *cpu;
            int screen[61440];
            
            char renderSprites[8];
        private:
            uint8_t reverseBits(uint8_t n);

            uint16_t nametableAddress;
            uint16_t patternEntryAddress;
            uint8_t attributeByte;
            uint8_t charTableEntryLsb;
            uint8_t charTableEntryMsb;

            void renderPixel();
            int scanLine;
            int cycle;
            
            union
            {
                struct
                {
                    uint8_t unused: 5;
                    uint8_t spriteOverflow: 1;
                    uint8_t spriteZeroHit: 1;
                    uint8_t verticalBlank: 1;
                };
                uint8_t reg;
            }
            status;
            
            union
            {
                struct
                {
                    uint8_t grayscale: 1;
                    uint8_t renderBackgroundLeft: 1;
                    uint8_t renderSpritesLeft: 1;
                    uint8_t renderBackground: 1;
                    uint8_t renderSprites: 1;
                    uint8_t enhanceRed: 1;
                    uint8_t enhanceGreen: 1;
                    uint8_t enhanceBlue: 1;
                };
                uint8_t reg;
            }
            mask;
            
            union
            {
                struct
                {
                    uint8_t nametableX: 1;
                    uint8_t nametableY: 1;
                    uint8_t incrementMode: 1;
                    uint8_t patternSprite: 1;
                    uint8_t patternBackground: 1;
                    uint8_t spriteSize: 1;
                    uint8_t slaveMode: 1;
                    uint8_t enableNmi: 1;
                };
                uint8_t reg;        
            }
            control;
        };

    }
}
#endif

