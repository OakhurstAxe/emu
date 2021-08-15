#ifndef _OA_NES_CARTRIDGE_H
#define _OA_NES_CARTRIDGE_H

#include "../oaemumemory/headers/memoryrom.h"

namespace oa
{
    namespace nes
    {
        
        class NesCartridge
        {
        public:
            virtual uint8_t CpuRead(uint16_t location) = 0;
            virtual void CpuWrite(uint16_t location, uint8_t byte) = 0;
            virtual uint8_t PpuRead(uint16_t location) = 0;
            virtual void PpuWrite(uint16_t location, uint8_t byte) = 0;
            virtual void LoadProgRom(uint8_t* data, uint8_t size) = 0;
            virtual void LoadCharRom(uint8_t* data, uint8_t size) = 0;
            virtual uint16_t GetMapper() = 0;
            static NesCartridge* GetCartridge(uint16_t mapper);
        };
       
    }
}

#endif

