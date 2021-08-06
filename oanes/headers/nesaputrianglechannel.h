#ifndef _OA_NES_APUTRIANGLECHANNEL_H
#define _OA_NES_APUTRIANGLECHANNEL_H

#include <QIODevice>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>

#include "headers/nesapuchannel.h"

namespace oa
{
    namespace nes
    {
        
        class NesApuTriangleChannel : public NesApuChannel
        {
            Q_OBJECT

        public:
            NesApuTriangleChannel();
            virtual ~NesApuTriangleChannel() override;

            void SetChannelSettings(uint8_t register1, uint8_t register2, uint8_t register3, uint8_t register4) override;
            float *GenerateBufferData(int sampleCount) override;
        protected:
            int frequency_ = 0;
            float counter_ = 0;
            bool reverse_ = false;
            int loadCounter_ = 0;
            bool controlFlag_ = false;
            int totalSamples_ = 0;
            bool haltFlag_ = true;
        };

    }
}
#endif

