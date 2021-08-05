#ifndef _OA_NES_APUPULSECHANNEL_H
#define _OA_NES_APUPULSECHANNEL_H

#include <QIODevice>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>

#include "headers/nesapuchannel.h"

namespace oa
{
    namespace nes
    {
        
        class NesApuPulseChannel : public NesApuChannel
        {
            Q_OBJECT

        public:
            NesApuPulseChannel();
            virtual ~NesApuPulseChannel();

            void PlaySound(uint8_t register1, uint8_t register2, uint8_t register3, uint8_t register4) override;
            void GenerateBufferData(int sampleCount) override;
        protected:
            int totalSample = 0;
            int frequency_ = 0;
            int duty_ = 0;
            qreal volume_ = 0;
        };

    }
}
#endif

