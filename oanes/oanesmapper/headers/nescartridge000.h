#ifndef _OA_NES_CARTRIDGE000_H
#define _OA_NES_CARTRIDGE000_H

#include "oaemumemory/headers/memoryram.h"
#include "oaemumemory/headers/memoryrom.h"
#include "nescartridge.h"

namespace oa
{
    namespace nes
    {
        
        class NesCartridge000 : public NesCartridge
        {
        public:
            NesCartridge000();
            virtual ~NesCartridge000();
            
            uint8_t CpuRead(uint16_t location) override;
            void CpuWrite(uint16_t location, uint8_t byte) override;
            uint8_t PpuRead(uint16_t location) override;
            void PpuWrite(uint16_t location, uint8_t byte) override;
            void LoadProgRom(uint8_t* data, uint8_t size) override;
            void LoadCharRom(uint8_t* data, uint8_t size) override;
            uint16_t GetMapper() override;
        private:
            emu::MemoryRom* cpuPrgRom_[2];
            emu::MemoryRom* ppuCharRom_[2];
        };
        
    }
}

#endif

