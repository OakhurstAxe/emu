#include <QDebug>
#include <qendian.h>
#include <math.h>

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

        void NesApuPulseChannel::PlaySound(uint8_t register1, uint8_t register2, uint8_t register3, uint8_t register4)
        {
            qreal volume = ((qreal)(register1 & 0x0f)) / 16;
                
            int timer = register3;
            timer += ((register4 & 0x07) << 8);
            if (timer < 8)
            {
                return;
            }
            int newFrequency = 1789773/(16 * (timer + 1));
            if (newFrequency < 30)
            {
                return;
            }
            int duty = (register1 & 0xc0) >> 6;
            frequency_ = newFrequency;
            volume_ = volume;
            duty_ = duty;
        }

        void NesApuPulseChannel::GenerateBufferData(int sampleCount)
        {
            int sampleIndex = 0;
            
            int dutyValue = 0;
            switch (duty_)
            {
                case (0):
                    dutyValue = 7;
                    break;
                case (1):
                    dutyValue = 6;
                    break;
                case (2):
                    dutyValue = 4;
                    break;
                case (4):
                    dutyValue = 2;
                    break;
            }
            if (frequency_ == 0)
                return;
            int wavelength = ((DataSampleRateHz / frequency_));
            int wavelengthEigth = wavelength / 8;
            while (sampleIndex < sampleCount) {
                float x = 0;
                //x = sin(sampleIndex * wavelength * 3.141 / 180);
                if ((totalSample % wavelength) < (wavelengthEigth * dutyValue))
                {
                    x = 1.0;
                }
                else 
                {
                    x = -1.0;
                }
                float value = (x * volume_);
                
                m_buffer[sampleIndex] = value;
                ++sampleIndex;
                totalSample++;
            }
            //qDebug() << "volume: " << volume_ << " frequency: " << frequency_;
        }

    }
}
