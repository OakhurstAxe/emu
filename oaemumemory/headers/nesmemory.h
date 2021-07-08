#ifndef _OA_EMU_NESMEMORY_H
#define _OA_EMU_NESMEMORY_H

#include "memoryram.h"
#include "memoryrom.h"

namespace oa
{
    namespace emu
    {
        
        class NesMemory
        {
        public:
            NesMemory();
            ~NesMemory();
            void LoadProgRom(uint8_t* data, uint16_t size);
            uint8_t CpuRead(unsigned short location);
            void CpuWrite(uint16_t location, uint8_t byte);
            void CpuSetVblank(uint8_t value);
            
            void LoadCharRom(uint8_t* data, uint16_t size);
            uint8_t PpuRead(uint16_t location);
            uint8_t PpuOamRead(uint16_t location);
            void PpuWrite(uint16_t location, uint8_t byte);
            uint8_t leftController_ = 0;
            uint8_t loadController_ = 0;
        private:
            MemoryRam* cpuWorkRam_;
            MemoryRam* cpuPpuRegisters_;
            MemoryRam* cpuApuIoRegisters_;
            MemoryRom* cpuPrgRom_;
            MemoryRom* cpuCartridgeRom_;
            
            MemoryRom* ppuCharRom_;
            MemoryRam* ppuNameTable_;
            MemoryRam* ppuPalette_;
            MemoryRam* ppuOam_;
            
            uint8_t ppuAddrH_;
            uint8_t ppuAddrL_;
            uint16_t ppuAddr_;
            int ppuAddrCount_ = 0;
            
            uint8_t ppuOamAddr_ = 0;
        };

    }
}
#endif
