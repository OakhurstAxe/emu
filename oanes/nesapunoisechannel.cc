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
            m_format.setSampleRate(DataSampleRateHz);
            m_format.setChannelCount(1);
            m_format.setSampleSize(16);
            m_format.setCodec("audio/pcm");
            m_format.setByteOrder(QAudioFormat::LittleEndian);
            m_format.setSampleType(QAudioFormat::SignedInt);
            
            qreal volume = ((qreal)(register1 & 0x0f)) / 16;
            SetVolume(volume);
                
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
            
            if (m_format.isValid())
            {
                if (frequency != newFrequency)
                    GenerateBufferData(m_format, newFrequency);
                frequency = newFrequency;
                WriteAudioOutput();
            }
        }

        void NesApuNoiseChannel::GenerateBufferData(const QAudioFormat &format, int frequency)
        {
            const int channelBytes = format.sampleSize() / 8;
            const int sampleBytes = channelBytes;

            Q_ASSERT(BufferSize % sampleBytes == 0);
            Q_UNUSED(sampleBytes) // suppress warning in release builds

            unsigned char *ptr = reinterpret_cast<unsigned char *>(m_buffer->data());
            int sampleIndex = 0;
            int length = BufferSize;
            
            while (length) {
                qreal x = 0;
                if ((sampleIndex / (format.sampleRate() / frequency/2)) % 2 == 0)
                {
                    x = 1;
                }
                else 
                {
                    x = 0;
                }           
                qint16 value = static_cast<qint16>(x * 32767);
                if (format.byteOrder() == QAudioFormat::LittleEndian)
                    qToLittleEndian<qint16>(value, ptr);
                else
                    qToBigEndian<qint16>(value, ptr);

                ptr += channelBytes;
                length -= channelBytes;

                ++sampleIndex;
            } 
        }

    }
}

