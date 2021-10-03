#ifndef _OA_VCS_AUDIOCHANNEL_H
#define _OA_VCS_AUDIOCHANNEL_H

#include "portaudio.h"

const int DataSampleRateHz        = 44100;
const int SamplesPerFrame         = 736;
const int SamplesPerHalfFrame     = 366;
const int SamplesPerQuarterFrame  = 183;
const int BufferSize              = SamplesPerFrame * sizeof(float);

namespace oa
{
    namespace vcs
    {
        enum ShiftRegister { four, five, nine, fiveToFour, div2, div31, div31four };


        class VcsAudioChannel
        {
        public:
            VcsAudioChannel();
            virtual ~VcsAudioChannel();

            static int pa_callback_mapper(
                const void* input, void* output,
                unsigned long frameCount,
                const PaStreamCallbackTimeInfo* timeInfo,
                PaStreamCallbackFlags statusFlags,
                void* userData);
            void SetChannelSettings(uint8_t volumeReg, uint8_t frequencyReg, uint8_t noiseReg);
            float *GenerateBufferData(int sampleCount);
        protected:
            inline static bool isInitalized_ = false;
            void ShiftRegisters();
            void ShiftFourRegister();
            void ShiftFiveRegister();
            void ShiftNineRegister();
            uint64_t totalSample_ = 0;
            qreal    volume_ = 0;
            uint16_t frequency_ = 0;
            
            float volumeSteps_ [16] = {0.000, 0.067, 0.134, 0.201, 0.268, 0.335, 0.402, 0.469, 
                                       0.535, 0.602, 0.669, 0.736, 0.803, 0.870, 0.937, 1.000};
            float m_buffer_[BufferSize];
            PaStream *stream_;
            bool isShutdown_ = false;
            
            ShiftRegister shiftRegister_ = four;
            bool applyThird_ = false;
            uint8_t thirdCount_ = 0;
            uint16_t div2Count_ = 0;
            uint16_t div31Count_ = 0;
            uint16_t shift4Register_ = 0xFFFF;
            uint16_t shift5Register_ = 0xFFFF;
            uint16_t shift9Register_ = 0xFFFF;
        };

    }
}
#endif

