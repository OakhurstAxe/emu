
#include "headers/nesppu.h"

void NesPpu::renderPixel()
{
    int screenScanLine = scanLine - 1;
    int screenCycle = cycle - 1;
    unsigned char ppuController = memory->cpuRead(0x2000);
    unsigned char attributeValue = 0;

    if (screenCycle == 0)
    {
        for (int i=0; i<8; i++)
        {
            renderSprites[i] = -1;
        }
        int spriteCount = 0;
        for (int i=0; i<64; i++)
        {
            unsigned char yPos = memory->ppuOamRead(i*4);
            if (screenScanLine - yPos >= 0 && screenScanLine - yPos < 8)
            {
                renderSprites[spriteCount] = i;
                spriteCount++;
                if (spriteCount == 8)
                {
                    break;
                }
            }            
        }
    }
    
    if (screenCycle == 254)
    {
        unsigned char background = memory->ppuRead(0x3f00);
        for (int i=0; i<8; i++)
        {
            int spritePos = renderSprites[i];
            if (spritePos == -1)
            {
                continue;
            }
            int yPos = memory->ppuOamRead(spritePos*4);
            unsigned short patternAddress = memory->ppuOamRead(spritePos*4 + 1) << 4;
            unsigned char attr = memory->ppuOamRead(spritePos*4 + 2);
            int xPos = memory->ppuOamRead(spritePos*4 + 3);
            unsigned char spriteLsb = 0;
            unsigned char spriteMsb = 0;
            if ((attr & 0x80) == 0) // flip verticle
            {
                spriteLsb = memory->ppuRead(patternAddress | ((screenScanLine - yPos) & 0x07));
                spriteMsb = memory->ppuRead(patternAddress | ((screenScanLine - yPos) & 0x07) | 0x08);
            }
            else
            {
                spriteLsb = memory->ppuRead(patternAddress | ((7 - screenScanLine - yPos) & 0x07));
                spriteMsb = memory->ppuRead(patternAddress | ((7 - screenScanLine - yPos) & 0x07) | 0x08);
            }
            if ((attr & 0x40) > 0)
            {
                spriteLsb = reverseBits(spriteLsb);
                spriteMsb = reverseBits(spriteMsb);
            }
            for (int j=xPos; j<xPos+8; j++)
            {
                unsigned char pixel = ((spriteMsb & 0x80) >> 6) | ((spriteLsb & 0x80) >> 7);
                unsigned char palette = (attr & 0x03) + 0x04;
                if (pixel != 0)
                {
                    unsigned char color = memory->ppuRead(pixel + (palette << 2) + 0x3f00);
                    if (background != color)
                    {
                        screen[screenScanLine * 256 + j] = color;
                    }
                }
                spriteLsb <<= 1;
                spriteMsb <<= 1;
            }
        }
    }
    
    if ((screenCycle % 32) == 0)
    {
        unsigned short attributeAddress = ((screenScanLine / 32) * 8 + (screenCycle / 32)) + 0x23c0 + (0x0400 * (ppuController & 0x03));
        attributeByte = memory->ppuRead(attributeAddress);
    }
    if ((screenCycle & 0x07) == 0)
    {
        unsigned short line = screenScanLine / 8;
        unsigned short cycle = screenCycle / 8;
        nametableAddress = (((line) * 32) + (cycle)) + 0x2000 + (0x0400 * (ppuController & 0x03));
        patternEntryAddress = (memory->ppuRead(nametableAddress) << 4) + 0x1000;
        charTableEntryLsb = memory->ppuRead(patternEntryAddress | (screenScanLine & 0x07));
        charTableEntryMsb = memory->ppuRead(patternEntryAddress | (screenScanLine & 0x07) | 0x08);
    }
    if (((screenScanLine % 32) < 16) && ((screenCycle % 32) < 16))
    {
        attributeValue = attributeByte & 0x03;
    }
    else if (((screenScanLine % 32) < 16) && ((screenCycle % 32) >= 16))
    {
        attributeValue = (attributeByte & 0x0c) >> 2;
    }
    else if (((screenScanLine % 32) >= 16) && ((screenCycle % 32) < 16))
    {
        attributeValue = (attributeByte & 0x30) >> 4;
    }
    else if (((screenScanLine % 32) >= 16) && ((screenCycle % 32) >= 16))
    {
        attributeValue = (attributeByte & 0xc0) >> 6;
    }
    unsigned char pixel = ((charTableEntryMsb & 0x80) >> 6) | ((charTableEntryLsb & 0x80) >> 7);
    unsigned short pixelAddress = 0x3f00;
    pixelAddress += pixel + (attributeValue << 2);
    int color = memory->ppuRead(pixelAddress);
    screen[screenScanLine * 256 + screenCycle] = color;
    charTableEntryLsb = charTableEntryLsb << 1;
    charTableEntryMsb = charTableEntryMsb << 1;
}

unsigned char NesPpu::reverseBits(unsigned char n) 
{
    unsigned char ans = 0;
    for(int i = 7; i >= 0; i--){
        ans |= (n & 1) <<i;
        n>>=1;
    }
    return ans;
}
   
int NesPpu::executeTicks(int count)
{
    int cycleCount = 0;
    while (cycleCount < count)
    {
        cycle++;
        if (cycle >= 341)
        {
            cycle = 0;
            scanLine++;
            if (scanLine >= 261)
            {
                scanLine = -1;
            }
        }
        
        if (scanLine > 0 && scanLine < 240 && cycle > 0 && cycle < 256)
        {
            renderPixel();
        }
        
        if (scanLine == 240 && cycle == 1)
        {
            memory->cpuSetVblank(1);
            control.reg = memory->cpuRead(0x2000);
            if (control.enableNmi)
            {
                cpu->nmiSet = 1;
            }
        }
        if (scanLine == 260 && cycle == 1)
        {
            memory->cpuSetVblank(0);
        }
        cycleCount++;
    }
    
    return cycleCount;
}

void NesPpu::reset()
{
    memory->cpuWrite(0x2000, 0x80);
    memory->cpuWrite(0x2001, 0);
    memory->cpuWrite(0x2002, 0);
    memory->cpuWrite(0x2003, 0);
    memory->cpuWrite(0x2004, 0);
    memory->cpuWrite(0x2005, 0);
    memory->cpuWrite(0x2006, 0);
    memory->cpuWrite(0x2007, 0);
    scanLine = -1;
    cycle = 0;
}
