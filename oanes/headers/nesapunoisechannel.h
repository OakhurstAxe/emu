#ifndef _OA_NES_APUNOISECHANNEL_H
#define _OA_NES_APUNOISECHANNEL_H

#include "headers/nesapuchannel.h"

namespace oa
{
    namespace nes
    {
        
        class NesApuNoiseChannel : public NesApuChannel
        {
        public:
            NesApuNoiseChannel();
            virtual ~NesApuNoiseChannel() override;

            void SetChannelSettings(uint8_t register1, bool register1flag,
                                    uint8_t register2, bool register2flag,
                                    uint8_t register3, bool register3flag,
                                    uint8_t register4, bool register4flag);
            float *GenerateBufferData(int sampleCount) override;
        protected:
            uint64_t totalSample_ = 0;
            
            bool haltFlag_ = false;
            bool constantVolume_ = false;
            uint8_t volume_ = 0;
            bool loopNoise = false;
            uint8_t noisePeriod_ = 0;
            uint16_t timerPeriod_ = 0;
            uint16_t loadCounter_ = 0;
            
            int frequency = 0;
            
            uint16_t timerPeriodLookup_ [16] = { 4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068 };
            union
            {
                struct
                {
                    uint8_t volume_: 3;
                    bool    constandVolume_: 1;
                    bool    haltFlag_: 1;
                };
                uint8_t register_;
            }
            volumeRegister_;

            union
            {
                struct
                {
                    uint8_t noisePeriod_: 3;
                    uint8_t unused_: 3;
                    bool    loopNoise_: 1;
                };
                uint8_t register_;
            }
            loopRegister_;

            union
            {
                struct
                {
                    uint8_t  unused_: 3;
                    uint16_t loadCounter_: 5;
                };
                uint8_t register_;
            }
            loadCounterRegister_;
            
        };

    }
}
#endif


