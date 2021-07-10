
#include <QDebug>
#include <qendian.h>

#include "headers/nesapuchannel.h"

const int DurationSeconds = 1;
const int ToneSampleRateHz = 600;
const int DataSampleRateHz = 44100;
const int BufferSize      = 32768;

namespace oa
{
    namespace nes
    {
        
        EmuApuChannel::EmuApuChannel() : 
              m_device(QAudioDeviceInfo::defaultOutputDevice())
            , m_audioOutput(0)
        {
            m_pos = 0;
        }

        EmuApuChannel::~EmuApuChannel()
        {
        }

        void EmuApuChannel::start()
        {
            m_audioOutput->start(this);
        }
            
        void EmuApuChannel::stop()
        {
            if (m_audioOutput != 0)
            {
                m_audioOutput->stop();
            }
        }

        qint64 EmuApuChannel::readData(char *data, qint64 len)
        {
            qint64 total = 0;
            if (!m_buffer[currentBuffer].isEmpty()) {
                while (len - total > 0) {
                    const qint64 chunk = qMin((m_buffer[currentBuffer].size() - m_pos), len - total);
                    memcpy(data + total, m_buffer[currentBuffer].constData() + m_pos, chunk);
                    m_pos = (m_pos + chunk) % m_buffer[currentBuffer].size();
                    total += chunk;
                }
            }
            return total;
        }
            
        qint64 EmuApuChannel::writeData(const char *data, qint64 len)
        {
            Q_UNUSED(data);
            Q_UNUSED(len);

            return 0;    
        }

        qint64 EmuApuChannel::bytesAvailable() const
        {
            return m_buffer[currentBuffer].size() + QIODevice::bytesAvailable();
        }

        void EmuApuChannel::playSound(qint64 durationUs, int sampleRate)
        {
            m_format.setSampleRate(DataSampleRateHz);
            m_format.setChannelCount(1);
            m_format.setSampleSize(16);
            m_format.setCodec("audio/pcm");
            m_format.setByteOrder(QAudioFormat::LittleEndian);
            m_format.setSampleType(QAudioFormat::SignedInt);
            
            QAudioDeviceInfo info(m_device);
            if (!info.isFormatSupported(m_format)) {
                qWarning() << "Default format not supported - trying to use nearest";
                m_format = info.nearestFormat(m_format);
            }
            
            if (m_format.isValid())
                generateData(m_format, durationUs, sampleRate);
                
            createAudioOutput();
        }

        void EmuApuChannel::generateData(const QAudioFormat &format, qint64 durationUs, int frequency)
        {
            const int channelBytes = format.sampleSize() / 8;
            const int sampleBytes = channelBytes;

            qint64 length = (format.sampleRate() * (format.sampleSize() / 8))
                                * durationUs / 100000;

            if (length % 2 == 1)
                length--;
            
            Q_ASSERT(length % sampleBytes == 0);
            Q_UNUSED(sampleBytes) // suppress warning in release builds

            int newBuffer = 0;
            if (currentBuffer == 0)
            {
                newBuffer = 1;
            }
            
            m_buffer[newBuffer].resize(length);
            unsigned char *ptr = reinterpret_cast<unsigned char *>(m_buffer[newBuffer].data());
            int sampleIndex = 0;

            while (length) {
                //const qreal x = qSin(2 * M_PI * frequency * qreal(sampleIndex % format.sampleRate()) / format.sampleRate());
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
            currentBuffer = newBuffer;
        }

        void EmuApuChannel::createAudioOutput()
        {
            open(QIODevice::ReadWrite);
            if (m_audioOutput == 0)
            {
                m_audioOutput = new QAudioOutput(m_device, m_format, this);
                m_audioOutput->setVolume(qreal(0.5f));
            }
            m_audioOutput->start(this);
            //delete m_audioOutput;
            //m_audioOutput = 0;
            //m_audioOutput = new QAudioOutput(m_device, m_format, this);
            //QAudio::State state = m_audioOutput->state();
            //QAudio::Error error = m_audioOutput->error();
        }

        void EmuApuChannel::setVolume(qreal volume)
        {
            if (m_audioOutput != 0)
            {
                m_audioOutput->setVolume(volume);
            }
        }

    }
}
