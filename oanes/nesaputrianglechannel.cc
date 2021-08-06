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

        void NesApuTriangleChannel::SetChannelSettings(uint8_t register1, uint8_t register2, uint8_t register3, uint8_t register4)
        {
            Q_UNUSED (register2);
            
            if (register2 > 0)
            {
                haltFlag_ = true;
                loadCounter_ = (register1 & 0x7f);
            }
            controlFlag_ = (register1 & 0x80);
            if (!controlFlag_)
            {
                haltFlag_ = false;
            }
            
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

        float *NesApuTriangleChannel::GenerateBufferData(int sampleCount)
        {
            int sampleIndex = 0;
            int length = SamplesPerFrame;
            
            if (frequency_ == 0)
            {
                memset(m_buffer_, 0, sizeof(m_buffer_));
                return m_buffer_;
            }
            
            float waveLengthStep = (DataSampleRateHz / frequency_);
            float step = 4/waveLengthStep;
            while (sampleIndex < sampleCount) {
                if (counter_ >= 1.0)
                {
                    counter_ = 1.0;
                    reverse_ = false;
                }
                else if (counter_ <= -1.0)
                {
                    counter_ = -1.0;
                    reverse_ = true;
                }
                  
                if ((loadCounter_ == 0 && !haltFlag_) || haltFlag_)
                    m_buffer_[sampleIndex] = 0;
                else
                    m_buffer_[sampleIndex] = counter_ / 2;
                
                if (reverse_)
                    counter_ += step;
                else
                    counter_ -= step;
                    
                totalSamples_++;
                if (totalSamples_ % 183 == 0 && loadCounter_ > 0)
                {
                    loadCounter_ --;
                }

                length -= 1;
                ++sampleIndex;
            } 
            return m_buffer_;
        }

    }
}

