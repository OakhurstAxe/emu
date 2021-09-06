#include <QDebug>

#include "headers/vcsaudiochannel.h"

namespace oa
{
    namespace vcs
    {
        
        VcsAudioChannel::VcsAudioChannel()
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
                &VcsAudioChannel::pa_callback_mapper,
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

        VcsAudioChannel::~VcsAudioChannel()
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
        
        void VcsAudioChannel::SetChannelSettings(uint8_t volumeReg, uint8_t frequencyReg, uint8_t noiseReg)
        {
            volume_ = volumeSteps_[volumeReg & 0x0F];
            
            uint8_t frequencyDivider = (frequencyReg & 0x1F) + 1;
            frequency_ = 30000 / frequencyDivider;
            
            applyThird_ = false;
 
            switch (noiseReg & 0x0F)
            {
                case (0):
                case (11):
                    volume_ = 0;
                    break;
                case (1):
                    if (shiftRegister_ != four)
                    {
                        shift4Register_ = 0xFFFF;
                        shiftRegister_ = four;
                    }
                    break;
                case (2):
                    if (shiftRegister_ != div31four)
                    {
                        div31Count_ = 0;
                        shift4Register_ = 0xFFFF;
                        shiftRegister_ = div31four;
                    }
                    break;
                case(3):
                    if (shiftRegister_ != fiveToFour)
                    {
                        shift4Register_ = 0xFFFF;
                        shift5Register_ = 0xFFFF;
                        shiftRegister_ = fiveToFour;
                    }
                    break;
                case (4):
                case (5):
                    if (shiftRegister_ != div2)
                    {
                        div2Count_ = 0;
                        shiftRegister_ = div2;
                    }
                    break;
                case (6):
                case (10):
                    if (shiftRegister_ != div31)
                    {
                        div31Count_ = 0;
                        shiftRegister_ = div31;
                    }
                case (8):
                    if (shiftRegister_ != nine)
                    {
                        shift9Register_ = 0xFFFF;
                        shiftRegister_ = nine;
                    }
                    break;
                case (7):
                case (9):
                    if (shiftRegister_ != five)
                    {
                        shift5Register_ = 0xFFFF;
                        shiftRegister_ = five;
                    }
                    break;
                case (12):
                case (13):
                    if (shiftRegister_ != div2)
                    {
                        div2Count_ = 0;
                        shiftRegister_ = div2;
                        applyThird_ = true;
                        thirdCount_ = 0;
                    }
                case (14):
                    if (shiftRegister_ != div31)
                    {
                        div31Count_ = 0;
                        shiftRegister_ = div31;
                        applyThird_ = true;
                        thirdCount_ = 0;
                    }
                case (15):
                    if (shiftRegister_ != five)
                    {
                        shift5Register_ = 0xFFFF;
                        shiftRegister_ = five;
                        applyThird_ = true;
                        thirdCount_ = 0;
                    }                    
            }
            
        }
        
        void VcsAudioChannel::ShiftRegisters()
        {
            uint8_t newBit = 0;
            
            if (applyThird_)
            {
                thirdCount_++;
                if (thirdCount_ < 2)
                {
                    return;
                }
                thirdCount_ = 0;
            }
            
            if (shiftRegister_ == four)
            {
                // Four
                newBit = ((shift4Register_ & 0x0002) >> 1) ^ (shift4Register_ & 0x0001);
                shift4Register_ = shift4Register_ >> 1;
                if (newBit > 0)
                {
                    shift4Register_ |= 0x0008;
                }
                else
                {
                    shift4Register_ &= 0xFFF7;
                }
            }
            else if (shiftRegister_ == five)
            {
                // Five
                newBit = ((shift5Register_ & 0x0004) >> 2) ^ (shift5Register_ & 0x0001);
                shift5Register_ = shift5Register_ >> 1;
                if (newBit > 0)
                {
                    shift5Register_ |= 0x0010;
                }
                else
                {
                    shift5Register_ &= 0xFFEF;
                }
            }
            else if (shiftRegister_ == nine)
            {                
                // Nine
                newBit = ((shift9Register_ & 0x0010) >> 4) ^ (shift9Register_ & 0x0001);
                shift9Register_ = shift9Register_ >> 1;
                if (newBit > 0)
                {
                    shift9Register_ |= 0x0100;
                }
                else
                {
                    shift9Register_ &= 0xFEFF;
                }
            }
            else if (shiftRegister_ == fiveToFour)
            {
                bool shiftFour = (shift5Register_ & 0x0001) > 0;
                if (shiftFour)
                {
                    newBit = ((shift4Register_ & 0x0002) >> 1) ^ (shift4Register_ & 0x0001);
                    shift4Register_ = shift4Register_ >> 1;
                    if (newBit > 0)
                    {
                        shift4Register_ |= 0x0008;
                    }
                    else
                    {
                        shift4Register_ &= 0xFFF7;
                    }
                }                
                newBit = ((shift5Register_ & 0x0004) >> 2) ^ (shift5Register_ & 0x0001);
                shift5Register_ = shift5Register_ >> 1;
                if (newBit > 0)
                {
                    shift5Register_ |= 0x0010;
                }
                else
                {
                    shift5Register_ &= 0xFFEF;
                }
            }
            else if (shiftRegister_ == div2)
            {
                if (div2Count_ == 0)
                {
                    div2Count_ = 1;
                }
                else
                {
                    div2Count_ = 0;
                }
            }
            else if (shiftRegister_ == div31)
            {
                if (div31Count_ >= 31)
                {
                    div31Count_ = 0;
                }
            }
            else if (shiftRegister_ == div31four)
            {
                div31Count_++;
                
                if (div31Count_ == 18 || div31Count_ == 31)
                {
                    // Four
                    newBit = ((shift4Register_ & 0x0002) >> 1) ^ (shift4Register_ & 0x0001);
                    shift4Register_ = shift4Register_ >> 1;
                    if (newBit > 0)
                    {
                        shift4Register_ |= 0x0008;
                    }
                    else
                    {
                        shift4Register_ &= 0xFFF7;
                    }
                }
                if (div31Count_ == 31)
                {
                    div31Count_ = 0;
                }
            }
        }

        float *VcsAudioChannel::GenerateBufferData(int sampleCount)
        {
            if (frequency_ == 0)
            {
                memset(m_buffer_, 0, sampleCount);
                return m_buffer_;
            }
            
            uint wavelength = ((DataSampleRateHz / frequency_));
            uint wavelengthEigth = wavelength / 8;
            int sampleIndex = 0;

            while (sampleIndex < sampleCount) 
            {
                if ((totalSample_ % wavelength) == 0)//wavelengthEigth / 2)
                {
                    ShiftRegisters();
                }
                
                if (shiftRegister_ == four 
                    || shiftRegister_ == div31four
                    || shiftRegister_ == fiveToFour)
                {
                    if ((shift4Register_ & 0x0001) == 1)
                    {
                        m_buffer_[sampleIndex] = volume_;
                    }
                    else
                    {
                        m_buffer_[sampleIndex] = -volume_;
                    }
                }
                else if (shiftRegister_ == five)
                {
                    if ((shift5Register_ & 0x0001) == 1)
                    {
                        m_buffer_[sampleIndex] = volume_;
                    }
                    else
                    {
                        m_buffer_[sampleIndex] = -volume_;
                    }
                }
                else if (shiftRegister_ == nine)
                {
                    if ((shift9Register_ & 0x0001) == 1)
                    {
                        m_buffer_[sampleIndex] = volume_;
                    }
                    else
                    {
                        m_buffer_[sampleIndex] = -volume_;
                    }
                }
                else if (shiftRegister_ == div2)
                {
                    if (div2Count_ == 1)
                    {
                        m_buffer_[sampleIndex] = volume_;
                    }
                    else
                    {
                        m_buffer_[sampleIndex] = -volume_;
                    }
                }
                else if (shiftRegister_ == div31)
                {
                    if (div31Count_ < 18)
                    {
                        m_buffer_[sampleIndex] = volume_;
                    }
                    else
                    {
                        m_buffer_[sampleIndex] = -volume_;
                    }
                }                
                sampleIndex++;
                totalSample_++;
            }
            return m_buffer_;
        }    

        int VcsAudioChannel::pa_callback_mapper(
            const void* input, void* output,
            unsigned long frameCount,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags,
            void* userData)
        {
            Q_UNUSED(input);
            Q_UNUSED(timeInfo);
            
            if(auto self = reinterpret_cast<VcsAudioChannel*>(userData))
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

    }
}

