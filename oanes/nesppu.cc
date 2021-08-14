
#include <QDebug>

#include "headers/nesppu.h"

#define PPU_CONTROL_ADDR    0x2000
#define PPU_MASK_ADDR       0x2001
#define PPU_STATUS_ADDR     0x2002
#define PPU_SCROLL_ADDR     0x2005
#define PPU_ADDR            0x2006
#define PPU_DATA_ADDR       0x2007
#define PPU_OAM_ADDR        0x2003
#define PPU_OAM_DATA_ADDR   0x2004
#define PPU_OAM_DMA_ADDR    0x4014

#define PPU_ATTRIBUTE_ADDR  0x23c0
#define PPU_ATTRIBUTE_SIZE  0x0040
#define PPU_NAMETABLE_ADDR  0x2000
#define PPU_NAMETABLE_SIZE  0x0400
#define PPU_PATTERN_SIZE    0x1000
#define PPU_PALETTE_ADDR    0x3f00

#define PPU_SPRITE_SIZE     0x0004
#define PPU_SPRITE_PATTERN_SIZE 0x0008

namespace oa
{
    namespace nes
    {
        
        NesPpu::NesPpu(NesMemory *memory)
        {
            memory_ = memory;
        }
        
        NesPpu::~NesPpu()
        {
        }
        
        void NesPpu::RenderPixel()
        {
            int screenScanLine = scanLine_ - 1;
            int screenCycle = cycle_;

            if (screenCycle == 0)
            {
                memset(renderSprites_, -1, 8);
                int spriteCount = 0;
                for (int i=0; i<64; i++)
                {
                    if (spriteCount == 8)
                    {
                        // sprite overflow
                        memory_->SetPpuSpriteOvervlow(1);
                        break;
                    }
                    
                    uint8_t yPos = memory_->PpuOamRead(i*4);
                    if (screenScanLine - yPos > 0 && screenScanLine - yPos <= 8)
                    {
                        if (i == 0)
                        {
                            //memory_->SetPpuSpriteZeroHit();
                        }                        
                        renderSprites_[spriteCount] = i;
                        spriteCount++;
                    }            
                }
            }
            
            if (screenCycle == 254)
            {
                uint8_t background = memory_->PpuRead(PPU_PALETTE_ADDR);
                for (int i=7; i>=0; i--)
                {
                    SpriteAttributeRegister spriteAttribute;
                    int spritePos = renderSprites_[i];
                    if (spritePos == -1)
                    {
                        continue;
                    }
                    int yPos = memory_->PpuOamRead(spritePos * PPU_SPRITE_SIZE) + 1;
                    uint16_t patternAddress = memory_->PpuOamRead(spritePos * PPU_SPRITE_SIZE + 1) << 4;
                    spriteAttribute.reg = memory_->PpuOamRead(spritePos * PPU_SPRITE_SIZE + 2);
                    int xPos = memory_->PpuOamRead(spritePos * PPU_SPRITE_SIZE + 3);
                    uint8_t spriteLsb = 0;
                    uint8_t spriteMsb = 0;
                    uint16_t spritePatternAddress = patternAddress + ((screenScanLine - yPos) & 0x07);
                    if (spriteAttribute.flipVertically) // flip verticle
                    {
                        spritePatternAddress = patternAddress + ((7 - screenScanLine - yPos) & 0x07);
                    }
                    spriteLsb = memory_->PpuRead(spritePatternAddress);
                    spriteMsb = memory_->PpuRead(spritePatternAddress + PPU_SPRITE_PATTERN_SIZE);
                    if (spriteAttribute.flipHorizontally)
                    {
                        spriteLsb = ReverseBits(spriteLsb);
                        spriteMsb = ReverseBits(spriteMsb);
                    }
                    for (int j=xPos; j<xPos+8; j++)
                    {
                        uint8_t pixel = ((spriteMsb >> 6) & 0x02) + ((spriteLsb >> 7) & 0x01);
                        uint8_t palette = ((spriteAttribute.spritePalette) + 0x04) << 2;
                        if (pixel != 0)
                        {
                            uint8_t color = memory_->PpuRead(PPU_PALETTE_ADDR + palette + pixel);
                            if (background != color)
                            {
                                if (spritePos == 0 && screen_[screenScanLine * 256 + j] != background)
                                {
                                    memory_->SetPpuSpriteZeroHit(1);
                                }
                                if (spriteAttribute.priority == 0 || screen_[screenScanLine * 256 + j] == background)
                                {
                                    screen_[screenScanLine * 256 + j] = color;
                                }
                            }
                        }
                        spriteLsb = spriteLsb << 1;
                        spriteMsb = spriteMsb << 1;
                    }
                }
            }
            
            uint backgroudPixel = GetBackgroundPixel(screenScanLine, screenCycle);
            if (cycle_ >= 0  && cycle_ < 256)
            {
                screen_[screenScanLine * 256 + screenCycle] = backgroudPixel;
            }
        }

        uint8_t NesPpu::GetBackgroundPixel(uint16_t screenRow, int16_t screenColumn)
        {
            uint8_t attributeValue = 0;
            uint8_t attributeShift = 0;
            controlRegister_.reg = memory_->CpuRead(PPU_CONTROL_ADDR);
            uint8_t xScroll = memory_->ppuXScroll_;
            uint8_t yScroll = memory_->ppuYScroll_;

            if (xScroll > 0 && screenRow == 10 && screenColumn == -1)
            {
                int x = 10;
            }
            // Donky Kong hammer barrel stop
            if (screenRow == 175 && screenColumn == 1)
            {
                //qDebug() << "xScroll: " << xScroll << " NametableX: " << controlRegister_.nametableX;
                int x = 10;
            }
            uint8_t nametableX = controlRegister_.nametableX;
            uint8_t nametableY = controlRegister_.nametableY;
            screenColumn += xScroll;
            
            if (screenColumn >= 256)
            {
                screenColumn -= 256;
                if (nametableX == 0)
                {
                    nametableX = 1;
                }
                else
                {
                    nametableX = 0;
                }
            }

            // Get attribute value
            // Should be one of: $23C0, $27C0, $2BC0, or $2FC0
            uint16_t attributeTableAddress = PPU_ATTRIBUTE_ADDR + (nametableX * 0x400) + (nametableY * 0x800);
            uint16_t attributeAddress = ((screenRow / 32) * 8 + (screenColumn / 32)) + attributeTableAddress;
            attributeByte_ = memory_->PpuRead(attributeAddress);

            if (((screenRow % 32) < 16) && (screenColumn % 32) < 16)
            {
                attributeShift = 0;
            }
            else if (((screenRow % 32) < 16) && (screenColumn % 32) >=- 16)
            {
                attributeShift = 2;
            }
            else if (((screenRow % 32) >= 16) && (screenColumn % 32) < 16)
            {
                attributeShift = 4;
            }
            else if (((screenRow % 32) >= 16) && (screenColumn % 32) >= 16)
            {
                attributeShift = 6;
            }
            attributeValue = ((attributeByte_  >> attributeShift) & 0x03);

            uint16_t tileRow = screenRow / 8;
            uint16_t tileColumn = (screenColumn / 8);
            // Should be one of: $2000, $2400, $2800, or $2C00
            uint16_t nametableTableAddress = PPU_NAMETABLE_ADDR + (nametableX * 0x400) + (nametableY * 0x800);
            nametableAddress_ = (((tileRow) * 32) + (tileColumn)) + nametableTableAddress;
            patternEntryAddress_ = ((memory_->PpuRead(nametableAddress_) << 4)  + (screenRow % 8)) + 
                PPU_PATTERN_SIZE * controlRegister_.patternBackground;
            charTableEntryLsb_ = memory_->PpuRead(patternEntryAddress_) << (screenColumn % 8);
            charTableEntryMsb_ = memory_->PpuRead(patternEntryAddress_ + 0x08) << (screenColumn % 8);
            uint8_t pixel = ((charTableEntryMsb_ & 0x80) >> 6) + ((charTableEntryLsb_ & 0x80) >> 7);
            uint16_t pixelAddress = PPU_PALETTE_ADDR + pixel + (attributeValue << 2);
            uint8_t color = memory_->PpuRead(pixelAddress);

            return color;
        }
        
        uint8_t NesPpu::ReverseBits(uint8_t n) 
        {
            uint8_t ans = 0;
            for(int i = 7; i >= 0; i--){
                ans |= (n & 1) <<i;
                n>>=1;
            }
            return ans;
        }
        
        void NesPpu::ExecuteTick()
        {
            cycle_++;
            if (cycle_ >= 340)
            {
                // Set rendering registers for when scrolling happens
                cycle_ = 0;
                scanLine_++;
                if (scanLine_ >= 261)
                {
                    scanLine_ = 0;
                }
            }
            
            memory_->SetPpuScanLineStatus(scanLine_);

            if (scanLine_ > 0 && scanLine_ <= 240 && cycle_ >= 0  && cycle_ <= 256)
            {
                RenderPixel();
            }
            
            if (scanLine_ == 241 && cycle_ == 1)
            {                
                memory_->CpuSetVblank(1);
                controlRegister_.reg = memory_->CpuRead(PPU_CONTROL_ADDR);
                if (controlRegister_.enableNmi)
                {
                    nmiSet_ = true;
                    memory_->ppuXScrollRead_ = true;
                    memory_->ppuXScrollWrite_ = true;
                }
            }
            if (scanLine_ == 261 && cycle_ == 1)
            {
                memory_->CpuSetVblank(0);
                memory_->SetPpuSpriteOvervlow(0);
                memory_->SetPpuSpriteZeroHit(0);
            }
        }

        bool NesPpu::IsNmiSet()
        {
            return nmiSet_;
        }
        
        void NesPpu::ResetNmi()
        {
            nmiSet_ = false;
        }

        void NesPpu::Reset()
        {
            memory_->CpuWrite(PPU_CONTROL_ADDR, 0x80);
            memory_->CpuWrite(PPU_MASK_ADDR, 0);
            memory_->CpuWrite(PPU_STATUS_ADDR, 0);
            memory_->CpuWrite(PPU_OAM_ADDR, 0);
            memory_->CpuWrite(PPU_OAM_DATA_ADDR, 0);
            memory_->CpuWrite(PPU_SCROLL_ADDR, 0);
            memory_->CpuWrite(PPU_ADDR, 0);
            memory_->CpuWrite(PPU_DATA_ADDR, 0);
            scanLine_ = -1;
            cycle_ = 0;
            nmiSet_ = false;
        }

        uint8_t* NesPpu::GetScreen()
        {
            return screen_;
        }
    }
}
