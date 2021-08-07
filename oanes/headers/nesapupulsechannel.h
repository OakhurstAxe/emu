#ifndef _OA_NES_APUPULSECHANNEL_H
#define _OA_NES_APUPULSECHANNEL_H

#include "headers/nesapuchannel.h"

namespace oa
{
    namespace nes
    {
        
        class NesApuPulseChannel : public NesApuChannel
        {
        public:
            NesApuPulseChannel();
            virtual ~NesApuPulseChannel() override;

            void SetChannelSettings(uint8_t register1, bool register1flag,
                                    uint8_t register2, bool register2flag,
                                    uint8_t register3, bool register3flag,
                                    uint8_t register4, bool register4flag);
            float *GenerateBufferData(int sampleCount) override;
            int GetDutyValue(int dutyReading);
        protected:
            uint64_t totalSample_ = 0;

            uint8_t  dutyValue_ = 0;
            bool     haltFlag_ = false;
            qreal    volume_ = 0;
            uint8_t  loadCounter_ = 0;
            uint16_t frequency_ = 0;
            uint16_t timer_ = 0;

            union
            {
                struct
                {
                    uint8_t volume_: 4;
                    bool    constantVolume_: 1;
                    bool    haltFlag_: 1;
                    uint8_t dutyReading_: 2;
                };
                uint8_t register_;
            }
            dutyRegister_;

            union
            {
                struct
                {
                    uint8_t shiftCount_: 3;
                    bool    negativeFlag_: 1;
                    uint8_t period_: 3;
                    bool    enabledFlag_: 1;
                };
                uint8_t register_;
            }
            sweepRegister_;
            
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
                    uint8_t timerHigh_: 3;
                    int     loadCounter_: 5;
                };
                uint8_t register_;
            }
            loadCounterRegister_;
        };

    }
}
#endif

