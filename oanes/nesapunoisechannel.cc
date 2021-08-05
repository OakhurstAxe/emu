#include <QDebug>
#include <qendian.h>

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

        void NesApuNoiseChannel::PlaySound(uint8_t register1, uint8_t register2, uint8_t register3, uint8_t register4)
        {
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
            
            if (frequency != newFrequency)
                GenerateBufferData(newFrequency);
            frequency = newFrequency;
        }

        void NesApuNoiseChannel::GenerateBufferData(int frequency)
        {
            const int channelBytes = 1;
            const int sampleBytes = channelBytes;

            Q_ASSERT(BufferSize % sampleBytes == 0);
            Q_UNUSED(sampleBytes) // suppress warning in release builds

            float ptr[SamplesPerFrame*sizeof(float)];
            int sampleIndex = 0;
            int length = BufferSize;
            
            while (length) {
                qreal x = 0;
                if ((sampleIndex / (DataSampleRateHz / frequency/2)) % 2 == 0)
                {
                    x = 1;
                }
                else 
                {
                    x = 0;
                }
                float value = static_cast<float>(x);
                ptr[length] = value;
                //qToLittleEndian<qint16>(value, ptr);

                //ptr += channelBytes;
                length -= channelBytes;

                ++sampleIndex;
            }
            memcpy(m_buffer, ptr, SamplesPerFrame*sizeof(float));
        }

    }
}

