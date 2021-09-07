#ifndef _OA_NES_APUCHANNEL_H
#define _OA_NES_APUCHANNEL_H

#include <QObject>

#include "portaudio.h"

const int DataSampleRateHz        = 44100;
const int SamplesPerFrame         = 736;
const int SamplesPerHalfFrame     = 366;
const int SamplesPerQuarterFrame  = 183;
const int BufferSize              = SamplesPerFrame * sizeof(float);

namespace oa
{
    namespace nes
    {
        
        class NesApuChannel
        {
        public:
            NesApuChannel();
            virtual ~NesApuChannel();

            static int pa_callback_mapper(
                const void* input, void* output,
                unsigned long frameCount,
                const PaStreamCallbackTimeInfo* timeInfo,
                PaStreamCallbackFlags statusFlags,
                void* userData);
            virtual void SetChannelSettings(uint8_t register1, bool register1flag,
                                            uint8_t register2, bool register2flag,
                                            uint8_t register3, bool register3flag,
                                            uint8_t register4, bool register4flag) = 0;
            virtual float *GenerateBufferData(int sampleCount);
        protected:
            uint16_t FrequencyFromTimer(uint16_t timer);
            float m_buffer_[BufferSize];
            float volumeSteps_ [16] = {0.000, 0.067, 0.134, 0.201, 0.268, 0.335, 0.402, 0.469, 
                                       0.535, 0.602, 0.669, 0.736, 0.803, 0.870, 0.937, 1.000};
            PaStream *stream_;
        };

    }
}
#endif
