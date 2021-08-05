#include <QDebug>
#include <qendian.h>

#include "headers/nesaputrianglechannel.h"

namespace oa
{
    namespace nes
    {
        
        NesApuTriangleChannel::NesApuTriangleChannel()
        {
        }

        NesApuTriangleChannel::~NesApuTriangleChannel()
        {
        }

        void NesApuTriangleChannel::PlaySound(uint8_t register1, uint8_t register2, uint8_t register3, uint8_t register4)
        {
            int timer = register3;
            timer += ((register4 & 0x07) << 8);
            if (timer < 8)
            {
                return;
            }
            int newFrequency = (1789773/(16 * (timer + 1)));
            if (newFrequency < 30)
            {
                return;
            }
            
            frequency_ = newFrequency;
        }

        void NesApuTriangleChannel::GenerateBufferData(int sampleCount)
        {
            float ptr[BufferSize];
            int sampleIndex = 0;
            int length = SamplesPerFrame;
            
            if (frequency_ == 0)
                return;
            
            float waveLengthStep = (DataSampleRateHz / frequency_);
            float step = 4/waveLengthStep;
            while (sampleIndex < sampleCount) {
                if (counter >= 1.0)
                {
                    counter = 1.0;
                    reverse = false;
                }
                else if (counter <= -1.0)
                {
                    counter = -1.0;
                    reverse = true;
                }
                    
                ptr[sampleIndex] = counter;
                
                if (reverse)
                    counter += step;
                else
                    counter -= step;
                    

                length -= 1;
                ++sampleIndex;
            } 
            //qDebug() << "triangle frequency: " << frequency_;            
            memcpy(m_buffer, ptr, BufferSize);
        }

    }
}

