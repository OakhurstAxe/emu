#ifndef _OA_NES_APUCHANNEL_H
#define _OA_NES_APUCHANNEL_H

#include <QIODevice>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>

#include "portaudio.h"

const int DataSampleRateHz = 44100;
const int SamplesPerFrame  = (661);
const int BufferSize       = SamplesPerFrame * sizeof(float);

namespace oa
{
    namespace nes
    {
        
        class NesApuChannel : public QObject
        {
            Q_OBJECT
            
        public:
            NesApuChannel();
            virtual ~NesApuChannel();

            static int pa_callback_mapper(
                const void* input, void* output,
                unsigned long frameCount,
                const PaStreamCallbackTimeInfo* timeInfo,
                PaStreamCallbackFlags statusFlags,
                void* userData);            
            virtual void SetChannelSettings(uint8_t register1, uint8_t register2, uint8_t register3, uint8_t register4) = 0;
            virtual float *GenerateBufferData(int sampleCount);
        protected:
            float m_buffer_[BufferSize];
            PaStream *stream_;
        };

    }
}
#endif
