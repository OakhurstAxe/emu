#ifndef _OA_NES_APUTRIANGLECHANNEL_H
#define _OA_NES_APUTRIANGLECHANNEL_H

#include "nesapuchannel.h"

namespace oa
{
    namespace nes
    {
        
        class NesApuTriangleChannel : public NesApuChannel
        {
        public:
            NesApuTriangleChannel();
            virtual ~NesApuTriangleChannel() override;

            void SetChannelSettings(uint8_t register1, bool register1flag,
                                    uint8_t register2, bool register2flag,
                                    uint8_t register3, bool register3flag,
                                    uint8_t register4, bool register4flag);
            float *GenerateBufferData(int sampleCount) override;
        protected:
            uint16_t timer_ = 0;
            uint16_t frequency_ = 0;
            float    counter_ = 0;
            bool     reverse_ = false;
            uint16_t loadCounter_ = 0;
            uint64_t totalSamples_ = 0;
            bool     haltFlag_ = true;
            
            union
            {
                struct
                {
                    uint16_t loadCounter_: 7;
                    bool     controlFlag_: 1;
                };
                uint8_t register_;
            }
            controlRegister_;

            union
            {
                struct
                {
                    uint16_t timer_: 8;
                };
                uint8_t register_;
            }
            timerRegister_;
            
            union
            {
                struct
                {
                    uint16_t timerHigh_: 3;
                    uint16_t lengthControl: 5;
                };
                uint8_t register_;
            }
            lengthCounterRegister_;
        };
        
    }
}
#endif

