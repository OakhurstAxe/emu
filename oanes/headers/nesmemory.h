#ifndef _OA_NES_NESMEMORY_H
#define _OA_NES_NESMEMORY_H

#include "../oaemumemory/headers/memoryram.h"
#include "../oaemumemory/headers/memoryrom.h"
#include "../oaemumemory/headers/memorymapper.h"

namespace oa
{
    namespace nes
    {
        
        class NesMemory: public emu::MemoryMapper
        {
        public:
            NesMemory();
            ~NesMemory();

            uint8_t CpuRead(unsigned short location) override;
            void CpuWrite(uint16_t location, uint8_t byte) override;
            uint8_t PpuRead(uint16_t location);
            void PpuWrite(uint16_t location, uint8_t byte);
            uint8_t PpuOamRead(uint16_t location);

            void CpuSetVblank(uint8_t value);
            bool IsLoadController();
            void SetLeftController(uint8_t byte);

            void LoadProgRom(uint8_t* data, uint16_t size);
            void LoadCharRom(uint8_t* data, uint16_t size);
        private:
            emu::MemoryRam* cpuWorkRam_;
            emu::MemoryRam* cpuPpuRegisters_;
            emu::MemoryRam* cpuApuIoRegisters_;
            emu::MemoryRom* cpuPrgRom_;
            
            emu::MemoryRom* ppuCharRom_;
            emu::MemoryRam* ppuNameTable_;
            emu::MemoryRam* ppuPalette_;
            emu::MemoryRam* ppuOam_;

            uint8_t leftController_ = 0;
            uint8_t loadController_ = 0;
            
            uint8_t ppuAddrH_;
            uint8_t ppuAddrL_;
            uint16_t ppuAddr_;
            int ppuAddrCount_ = 0;
            
            uint8_t ppuOamAddr_ = 0;
        };

    }
}
#endif
