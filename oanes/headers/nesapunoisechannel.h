#ifndef _OA_NES_APUNOISECHANNEL_H
#define _OA_NES_APUNOISECHANNEL_H

#include <QIODevice>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>

#include "headers/nesapuchannel.h"

namespace oa
{
    namespace nes
    {
        
        class NesApuNoiseChannel : public NesApuChannel
        {
            Q_OBJECT

        public:
            NesApuNoiseChannel();
            virtual ~NesApuNoiseChannel() override;

            void SetChannelSettings(uint8_t register1, uint8_t register2, uint8_t register3, uint8_t register4) override;
        protected:
            float *GenerateBufferData(int frequency) override;
            int frequency = 0;
        };

    }
}
#endif


