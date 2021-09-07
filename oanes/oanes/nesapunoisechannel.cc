#include <QDebug>

#include "headers/nesapunoisechannel.h"

namespace oa
{
    namespace nes
    {
        
        NesApuNoiseChannel::NesApuNoiseChannel()
        {
        }

        NesApuNoiseChannel::~NesApuNoiseChannel()
        {
        }

        void NesApuNoiseChannel::SetChannelSettings(uint8_t register1, bool register1flag,
                                                    uint8_t register2, bool register2flag,
                                                    uint8_t register3, bool register3flag,
                                                    uint8_t register4, bool register4flag)
        {
            Q_UNUSED (register2);
            Q_UNUSED(register1flag);
            Q_UNUSED(register2flag);
            Q_UNUSED(register3flag);
            Q_UNUSED(register4flag);

            volumeRegister_.register_ = register1;
            loopRegister_.register_ = register3;
            loadCounterRegister_.register_ = register4;
            
            haltFlag_ = volumeRegister_.haltFlag_;
            constantVolume_ = volumeRegister_.constandVolume_;
            volume_ = volumeRegister_.volume_;
            loopNoise = loopRegister_.loopNoise_;
            
            if (register3flag)
            {
                noisePeriod_ = loopRegister_.noisePeriod_;
                timerPeriod_ = timerPeriodLookup_[noisePeriod_];
            }
            
            if (register1flag)
            {
                loadCounter_ = loadCounterRegister_.loadCounter_;
                if (loadCounter_ == 1)
                {
                    loadCounter_ = 255;
                }
            }
        }

        float *NesApuNoiseChannel::GenerateBufferData(int sampleCount)
        {
            if (frequency == 0)
            {
                memset(m_buffer_, 0, sampleCount);
                return m_buffer_;
            }

            int sampleIndex = 0;
            uint16_t frequency = FrequencyFromTimer(timerPeriod_);
            uint wavelength = ((DataSampleRateHz / frequency));

            qDebug() << "Noise frequency: " << frequency;
            
            while (sampleIndex < sampleCount) 
            {
                if (loadCounter_ == 0 && haltFlag_ == false)
                {
                    m_buffer_[sampleIndex] = 0.0;
                }
                else if ((totalSample_ % wavelength) < (wavelength >> 1))
                {
                    m_buffer_[sampleIndex] = 1;
                }
                else 
                {
                    m_buffer_[sampleIndex] = 0;
                }

                if (totalSample_ % SamplesPerHalfFrame == 0) // 120 Hz timer
                {
                    uint8_t feedback = timerPeriod_ & 0x01;
                    if (noisePeriod_)
                    {
                        feedback = !feedback != !1;
                    }
                    else
                    {
                        feedback = !feedback != !(timerPeriod_ & 0x20);
                    }
                    timerPeriod_ = timerPeriod_ >> 1;
                    timerPeriod_ |= feedback << 15;
                    frequency = FrequencyFromTimer(timerPeriod_);
                    wavelength = ((DataSampleRateHz / frequency));
                    
                    if (loadCounter_ > 0 && haltFlag_ == false)
                    {
                        loadCounter_ --;
                    }
                }

                sampleIndex++;
                totalSample_++;
            }
            return m_buffer_;
        }

    }
}

