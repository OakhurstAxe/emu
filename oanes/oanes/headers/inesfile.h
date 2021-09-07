#ifndef _OA_NES_INESFILE_H
#define _OA_NES_INESFILE_H

#include <stdint.h>
#include <QString>

namespace oa
{
    namespace nes
    {
        class INesFile
        {
        public:
            void LoadFile(QString fileName);
            uint8_t *GetProgRomData();
            uint8_t GetProgRomSize();
            uint8_t *GetCharRomData();
            uint8_t GetCharRomSize();
            uint16_t GetMemoryMapper();

        private:
            uint16_t memoryMapper_ = 0;
            uint16_t progRomSize_;
            uint16_t charRomSize_;
            
            uint8_t *progRomData_;
            uint8_t *charRomData_;
            char header_[4];
            uint8_t progRomSizeLsb_;
            uint8_t charRomSizeLsb_;
            union
            {
            struct
                {
                    uint8_t nametableMirroring_: 1;
                    uint8_t batteryBackup_: 1;
                    uint8_t hasTrainer_: 1;
                    uint8_t fourScreenMode_: 1;
                    uint8_t mapperLsb_: 4;
                };
                uint8_t reg;
            }
            cartridgeFlags_;
            union
            {
                struct
                {
                    uint8_t consoleType_: 2;
                    uint8_t nes2Format_: 2;
                    uint8_t MapperCsb_: 4;
                };
                uint8_t reg;
            }
            consoleTypeFlags_;
            union
            {
                struct
                {
                    uint8_t mapperMsb_: 4;
                    uint8_t msbMapper_: 4;
                };
                uint8_t reg;
            }
            mapperFlags_;
            union
            {
                struct
                {
                    uint8_t prgRomSizeMsb_: 4;
                    uint8_t cahrRomSizeMsb_: 4;
                };
                uint8_t reg;
            }
            romSizeFlags_;
            union
            {
                struct
                {
                    uint8_t prgRamShiftCount_: 4;
                    uint8_t epromShiftCount_: 4;
                };
                uint8_t reg;
            }
            ramEpromSizeFlags_;
            union
            {
                struct
                {
                    uint8_t charRamShiftCount_: 4;
                    uint8_t charNvRamShiftCount_: 4;
                };
                uint8_t reg;
            }
            charRomSizeFlags_;
            uint8_t unusedPadding_;
            
            uint8_t trainer_[512];
            
        };
        
    }
}

#endif

