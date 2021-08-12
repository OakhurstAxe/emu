#ifndef _INESFILE_H
#define _INESFILE_H

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
            uint8_t *progRomData_;
            uint8_t *charRomData_;

        private:
            uint8_t memoryMapper_ = 0;
            char header_[4];
            uint8_t programRomSize_;
            uint8_t characterRomSize_;
            union
            {
            struct
                {
                    uint8_t mirroring: 1;
                    uint8_t batteryBackup: 1;
                    uint8_t hasTrainer: 1;
                    uint8_t ignoreMirroring: 1;
                    uint8_t lsbMapper: 4;
                };
                uint8_t reg;
            }
            cartridgeFlags_;
            union
            {
                struct
                {
                    uint8_t vsUnisystem: 1;
                    uint8_t isPlayChoice: 1;
                    uint8_t nes2Format: 2;
                    uint8_t msbMapper: 4;
                };
                uint8_t reg;
            }
            playChoiceFlags_;
            uint8_t programRamSize_;
            union
            {
                struct
                {
                    uint8_t prgRomSizeMsb_: 4;
                    uint8_t chrRomSizeMsb_: 4;
                };
                uint8_t reg;
            }
            romSize_;
            union
            {
                struct
                {
                    uint8_t tvSystem: 2;
                    uint8_t unused: 2;
                    uint8_t hasProgramRam: 1;
                    uint8_t hasBusConfilcts: 1;
                    uint8_t unused2: 2;
                };
                uint8_t reg;
            }
            tvSystemFlags_;
            uint8_t unusedPadding_;
            
            uint8_t trainer_[512];
            
        };
        
    }
}

#endif

