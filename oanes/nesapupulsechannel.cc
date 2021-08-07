#include <QDebug>

#include "headers/nesapupulsechannel.h"

namespace oa
{
    namespace nes
    {
        
        NesApuPulseChannel::NesApuPulseChannel()
        {
        }

        NesApuPulseChannel::~NesApuPulseChannel()
        {
        }

        void NesApuPulseChannel::SetChannelSettings(uint8_t register1, bool register1flag,
                                                    uint8_t register2, bool register2flag,
                                                    uint8_t register3, bool register3flag,
                                                    uint8_t register4, bool register4flag)
        {
            Q_UNUSED(register3flag);
            Q_UNUSED(register4flag);
            
            dutyRegister_.register_ = register1;
            timerRegister_.register_ = register3;
            loadCounterRegister_.register_ = register4;

            volume_ = volumeSteps_[dutyRegister_.volume_];
            dutyValue_ = GetDutyValue(dutyRegister_.dutyReading_);
            haltFlag_ = dutyRegister_.haltFlag_;

            if (register1flag)
            {
                loadCounter_ = loadCounterRegister_.loadCounter_;
                if (loadCounter_ == 1)
                {
                    loadCounter_ = 255;
                }
            }

            if (register2flag)
            {
                sweepRegister_.register_ = register2;
            }

            timer_ = (loadCounterRegister_.timerHigh_ << 8) + timerRegister_.timer_;
            frequency_ = FrequencyFromTimer(timer_);
        }

        int NesApuPulseChannel::GetDutyValue(int dutyReading)
        {
            int dutyResult = 4;
            switch (dutyReading)
            {
                case (0):
                    dutyResult = 7;
                    break;
                case (1):
                    dutyResult = 6;
                    break;
                case (2):
                    dutyResult = 4;
                    break;
                case (4):
                    dutyResult = 2;
                    break;
            }
            return dutyResult;
        }

        float *NesApuPulseChannel::GenerateBufferData(int sampleCount)
        {
            if (frequency_ == 0)
            {
                memset(m_buffer_, 0, sampleCount);
                return m_buffer_;
            }
            
            uint wavelength = ((DataSampleRateHz / frequency_));
            uint wavelengthEigth = wavelength / 8;
            uint sampleIndex = 0;

            while (sampleIndex < sampleCount) 
            {

                int x = 32;
                if (loadCounter_ == 0 && haltFlag_ == false)
                {
                    m_buffer_[sampleIndex] = 0.0;
                }
                else if ((totalSample_ % wavelength) < (wavelengthEigth * dutyValue_))
                {
                    m_buffer_[sampleIndex] = volume_;
                }
                else 
                {
                    m_buffer_[sampleIndex] = -volume_;
                }
                
                if (totalSample_ % SamplesPerHalfFrame == 0) // 120 Hz timer
                {
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
