#ifndef _OA_NES_PPU_H
#define _OA_NES_PPU_H

#include "../oaemucpu/headers/basecpu.h"

#include "../oanes/headers/nesmemory.h"
#include "r2a03.h"

namespace oa
{
    namespace nes
    {
        
        class NesPpu : emu::BaseCpu
        {
        public:
            NesPpu(NesMemory *memory);
            virtual ~NesPpu();
            void ExecuteTick();
            bool IsNmiSet();
            void ResetNmi();
            void Reset();
            uint8_t* GetScreen();
        private:
            uint8_t GetBackgroundPixel(uint16_t screenRow, int16_t screenColumn);
            uint8_t ReverseBits(uint8_t n);
            uint8_t screen_[61440];
            char renderSprites_[8];
            char spriteZero_;
            bool nmiSet_ = false;
            NesMemory *memory_;

            uint16_t nametableAddress_;
            uint16_t patternEntryAddress_;
            uint8_t attributeByte_;
            uint8_t charTableEntryLsb_;
            uint8_t charTableEntryMsb_;
            
            void RenderPixel();
            int scanLine_;
            int cycle_;
            
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
            statusRegister_;
            
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
            maskRegister_;
            
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
            controlRegister_;
            
            union SpriteAttributeRegister
            {
                struct 
                {
                    uint8_t spritePalette: 2;
                    uint8_t unimplemented: 3;
                    uint8_t priority: 1;
                    uint8_t flipHorizontally: 1;
                    uint8_t flipVertically: 1;
                };
                uint8_t reg;        
            };
        };

    }
}
#endif

