#include <QDebug>

#include "headers/nesapuchannel.h"

namespace oa
{
    namespace nes
    {
        
        NesApuChannel::NesApuChannel()
        {
            PaError err = Pa_Initialize();
            if (err != paNoError)
            {
                qDebug() << "Error starting port audio";
            }

            err = Pa_OpenDefaultStream(&stream_,
                0,
                1,
                paFloat32,
                DataSampleRateHz,
                0,
                &NesApuChannel::pa_callback_mapper,
                this);                
            if (err != paNoError)
            {
                qDebug() << "Error opening stream for port audio";
            }
            
            err = Pa_StartStream(stream_);
            if (err != paNoError)
            {
                qDebug() << "Error starting stream for port audio";
            }
                
        }

        NesApuChannel::~NesApuChannel()
        {
            PaError err = Pa_StopStream(stream_);
            if (err != paNoError)
            {
                qDebug() << "Error stopping stream for port audio";
            }

            err = Pa_Terminate();
            if (err != paNoError)
            {
                qDebug() << "Error terminating port audio";
            }
        }
        
        float *NesApuChannel::GenerateBufferData(int sampleCount)
        {
            Q_UNUSED(sampleCount);
            
            qDebug() << "Virtual GenerateBufferData called";
            return m_buffer_;
        }    

        int NesApuChannel::pa_callback_mapper(
            const void* input, void* output,
            unsigned long frameCount,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags,
            void* userData)
        {
            Q_UNUSED(input);
            Q_UNUSED(timeInfo);
            
            if(auto self = reinterpret_cast<NesApuChannel*>(userData))
            {
                if (statusFlags == 4)
                    qDebug() << "Audio underflow";
                //qDebug() << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                //qDebug() << "frameCount: " << frameCount;
                float *buffer = self->GenerateBufferData(frameCount);
                memcpy(output, buffer, frameCount * sizeof(float));
                return 0;
            }
            return 0;
        }
        
        uint16_t NesApuChannel::FrequencyFromTimer(uint16_t timer)
        {
            if (timer < 8)
            {
                return 0;
            }
            return 1789773/(16 * (timer + 1));
        }

    }
}
