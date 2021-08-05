#include <QDebug>
#include <qendian.h>

#include <chrono>
#include <iostream>
#include <sys/time.h>
#include <ctime>

#include "headers/nesapuchannel.h"
#include "headers/nesapupulsechannel.h"

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

            err = Pa_OpenDefaultStream(&stream,
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
            
            err = Pa_StartStream(stream);
            if (err != paNoError)
            {
                qDebug() << "Error starting stream for port audio";
            }
                
        }

        NesApuChannel::~NesApuChannel()
        {
            PaError err = Pa_StopStream(stream);
            if (err != paNoError)
            {
                qDebug() << "Error stopping stream for port audio";
            }

            err = Pa_Terminate();
            if (err != paNoError)
            {
                qDebug() << "Error starting port audio";
            }
        }

        void NesApuChannel::GenerateBufferData(int sampleCount)
        {
            memset(m_buffer, 0, BufferSize);
        }

        int NesApuChannel::pa_callback_mapper(
            const void* input, void* output,
            unsigned long frameCount,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags,
            void* userData)
        {
            if(auto self = reinterpret_cast<NesApuChannel*>(userData))
            {
                if (statusFlags == 4)
                    qDebug() << "Audio underflow";
                //qDebug() << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                //qDebug() << "frameCount: " << frameCount;
                self->GenerateBufferData(frameCount);
                memcpy(output, self->m_buffer, frameCount * sizeof(float));
                return 0;
            }
            return 0;
        }
        
    }
}
