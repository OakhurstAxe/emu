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
            m_format.setSampleRate(DataSampleRateHz);
            m_format.setChannelCount(1);
            m_format.setSampleSize(8);
            m_format.setCodec("audio/pcm");
            m_format.setByteOrder(QAudioFormat::LittleEndian);
            m_format.setSampleType(QAudioFormat::UnSignedInt);
            
            qreal volume = 0.25;
            SetVolume(volume);
                
            int timer = register3;
            timer += ((register4 & 0x07) << 8);
            if (timer < 8)
            {
                return;
            }
            int newFrequency = (1789773/(16 * (timer + 1)) / 2);
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

        void NesApuTriangleChannel::GenerateBufferData(const QAudioFormat &format, int frequency)
        {
            const int channelBytes = format.sampleSize() / 8;
            const int sampleBytes = channelBytes;

            Q_ASSERT(BufferSize % sampleBytes == 0);
            Q_UNUSED(sampleBytes) // suppress warning in release builds

            qint8 *ptr = reinterpret_cast<qint8 *>(m_buffer->data());
            int sampleIndex = 0;
            int length = BufferSize;
            
            qreal waveLengthStep = (format.sampleRate() / frequency/2);
            qreal counter = 0;
            bool reverse = 0;
            while (length) {
                if (counter >= 1 || counter <= 0)
                    reverse = ~reverse;
                
                qreal x = counter;
                
                if (reverse)
                    counter += 1.0/waveLengthStep;
                else
                    counter -= 1.0/waveLengthStep;
                    
                qint8 value = static_cast<qint8>(x * 500);
                if (format.byteOrder() == QAudioFormat::LittleEndian)
                    qToLittleEndian<qint8>(value, ptr);
                else
                    qToBigEndian<qint8>(value, ptr);

                ptr += channelBytes;
                length -= channelBytes;

                ++sampleIndex;
            } 
        }

    }
}

