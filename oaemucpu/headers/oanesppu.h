#ifndef _NESPPU_H
#define _NESPPU_H

#include "oabasecpu.h"

#include "../oaemumemory/headers/nesmemory.h"
#include "oam6502.h"

class NesPpu : BaseCpu
{
public:
    oa::emu::NesMemory *memory;
    int executeTicks(int count);
    void reset();
    M6502 *cpu;
    int screen[61440];
    
    char renderSprites[8];
private:
    unsigned char reverseBits(unsigned char n);

    unsigned short nametableAddress;
    unsigned short patternEntryAddress;
    unsigned char attributeByte;
    unsigned char charTableEntryLsb;
    unsigned char charTableEntryMsb;

    void renderPixel();
    int scanLine;
    int cycle;
    
    union
    {
        struct
        {
            unsigned char unused: 5;
            unsigned char spriteOverflow: 1;
            unsigned char spriteZeroHit: 1;
            unsigned char verticalBlank: 1;
        };
        unsigned char reg;
    }
    status;
    
    union
    {
        struct
        {
            unsigned char grayscale: 1;
            unsigned char renderBackgroundLeft: 1;
            unsigned char renderSpritesLeft: 1;
            unsigned char renderBackground: 1;
            unsigned char renderSprites: 1;
            unsigned char enhanceRed: 1;
            unsigned char enhanceGreen: 1;
            unsigned char enhanceBlue: 1;
        };
        unsigned char reg;
    }
    mask;
    
    union
    {
        struct
        {
            unsigned char nametableX: 1;
            unsigned char nametableY: 1;
            unsigned char incrementMode: 1;
            unsigned char patternSprite: 1;
            unsigned char patternBackground: 1;
            unsigned char spriteSize: 1;
            unsigned char slaveMode: 1;
            unsigned char enableNmi: 1;
        };
        unsigned char reg;        
    }
    control;
};

#endif

