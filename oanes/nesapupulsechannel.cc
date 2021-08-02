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
            m_format.setSampleRate(DataSampleRateHz);
            m_format.setChannelCount(1);
            m_format.setSampleSize(8);
            m_format.setCodec("audio/pcm");
            m_format.setByteOrder(QAudioFormat::LittleEndian);
            m_format.setSampleType(QAudioFormat::UnSignedInt);
            
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
            int duty = (register1 & 0xc0) >> 6;
            if (m_format.isValid())
            {
                if (frequency != newFrequency)
                {
                    GenerateBufferData(m_format, newFrequency, duty);
                }
                WriteAudioOutput();
                frequency = newFrequency;
            }
        }

        void NesApuPulseChannel::GenerateBufferData(const QAudioFormat &format, int frequency, int duty)
        {
            const int channelBytes = format.sampleSize() / 8;
            const int sampleBytes = channelBytes;

            Q_ASSERT(BufferSize % sampleBytes == 0);
            Q_UNUSED(sampleBytes) // suppress warning in release builds

            qint8 *ptr = reinterpret_cast<qint8 *>(m_buffer->data());
            int sampleIndex = 0;
            int length = BufferSize;
            
            int dutyValue = 0;
            switch (duty)
            {
                case (0):
                    dutyValue = 8;
                    break;
                case (1):
                    dutyValue = 4;
                    break;
                case (2):
                    dutyValue = 2;
                    break;
                case (4):
                    dutyValue = 4;
                    break;
            }
            int prevX = 0;
            qreal wavelength = ((format.sampleRate() / frequency));
            wavelength = wavelength / 360.0;
            while (length) {
                qreal x = 0;
                //x = sin(sampleIndex * wavelength * 3.141 / 180)+1.0; 
                
                if ((sampleIndex / (format.sampleRate() / frequency/2)) % dutyValue == 0)
                {
                    x = 1;
                }
                else 
                {
                    x = 0;
                }
                
                prevX = x;
                qint8 value = static_cast<qint8>(x * 200);
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
