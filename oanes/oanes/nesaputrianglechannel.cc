#include <QDebug>

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

        void NesApuTriangleChannel::SetChannelSettings(uint8_t register1, bool register1flag,
                                                       uint8_t register2, bool register2flag,
                                                       uint8_t register3, bool register3flag,
                                                       uint8_t register4, bool register4flag)
        {
            Q_UNUSED (register2);
            Q_UNUSED (register2flag);
            Q_UNUSED (register3flag);
            Q_UNUSED (register4flag);
            
            controlRegister_.register_ = register1;
            timerRegister_.register_ = register3;
            lengthCounterRegister_.register_ = register4;
            
            if (register1flag)
            {
                haltFlag_ = true;
                loadCounter_ = controlRegister_.loadCounter_;
            }
            if (!controlRegister_.controlFlag_)
            {
                haltFlag_ = false;
            }
            
            timer_ = (lengthCounterRegister_.timerHigh_ << 8) + timerRegister_.timer_;
            frequency_ = FrequencyFromTimer(timer_);
        }

        float *NesApuTriangleChannel::GenerateBufferData(int sampleCount)
        {
            int sampleIndex = 0;
            
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

                if (loadCounter_ == 0 && haltFlag_ == false)
                {
                    m_buffer_[sampleIndex] = 0.0;
                }                
                else
                {
                    m_buffer_[sampleIndex] = counter_ / 2;
                }
                
                if (reverse_)
                    counter_ += step;
                else
                    counter_ -= step;
                    
                totalSamples_++;
                if (totalSamples_ % SamplesPerQuarterFrame == 0) // 240 Hz counter
                {
                    if (loadCounter_ > 0 && haltFlag_ == false)
                    {
                        loadCounter_ --;
                    }
                }

                sampleIndex++;
            } 
            return m_buffer_;
        }

    }
}

