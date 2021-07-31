#include <QDebug>
#include <qendian.h>

#include "headers/nesapuchannel.h"

const int DurationSeconds  = 1;
const int ToneSampleRateHz = 600;
const int DataSampleRateHz = 44100;
const int BufferSize       = 8192;
const int SamplesPerFrame  = 1470;
const int Period           = 3528;
namespace oa
{
    namespace nes
    {
        
        NesApuChannel::NesApuChannel() : 
              m_device(QAudioDeviceInfo::defaultOutputDevice())
            , m_audioOutput(0)
        {
            m_pos = 0;
        }

        NesApuChannel::~NesApuChannel()
        {
        }

        qint64 NesApuChannel::readData(char *data, qint64 len)
        {
            Q_UNUSED(data);
            Q_UNUSED(len);

            return 0;
        }
            
        qint64 NesApuChannel::writeData(const char *data, qint64 len)
        {
            Q_UNUSED(data);
            Q_UNUSED(len);

            return 0;    
        }

        qint64 NesApuChannel::bytesAvailable() const
        {
            return m_buffer[currentBuffer].size() + QIODevice::bytesAvailable();
        }

        void NesApuChannel::playSound(int inFrequency)
        {
            qint64 durationUs = 25000;
            m_format.setSampleRate(DataSampleRateHz);
            m_format.setChannelCount(1);
            m_format.setSampleSize(16);
            m_format.setCodec("audio/pcm");
            m_format.setByteOrder(QAudioFormat::LittleEndian);
            m_format.setSampleType(QAudioFormat::SignedInt);
            
            //QAudioDeviceInfo info(m_device);
            //if (!info.isFormatSupported(m_format)) {
                //qWarning() << "Default format not supported - trying to use nearest";
                //m_format = info.nearestFormat(m_format);
            //}
            
            if (m_format.isValid())
            {
                if (frequency != inFrequency)
                    generateData(m_format, durationUs, inFrequency);
                frequency = inFrequency;
                createAudioOutput();
            }
        }

        void NesApuChannel::generateData(const QAudioFormat &format, qint64 durationUs, int frequency)
        {
            const int channelBytes = format.sampleSize() / 8;
            const int sampleBytes = channelBytes;

            Q_ASSERT(BufferSize % sampleBytes == 0);
            Q_UNUSED(sampleBytes) // suppress warning in release builds

            QByteArray *newBuffer = new QByteArray();
            newBuffer->resize(BufferSize);
            unsigned char *ptr = reinterpret_cast<unsigned char *>(newBuffer->data());
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
            m_buffer = newBuffer;
        }

        void NesApuChannel::createAudioOutput()
        {
            bufferSize -= SamplesPerFrame;
            if (bufferSize < 0)
                bufferSize = 0;

            open(QIODevice::ReadWrite);
            if (m_audioOutput == 0)
            {
                m_audioOutput = new QAudioOutput(m_device, m_format, this);
                m_audioOutput->setVolume(qreal(0.5f));
            }
            
            if (io == 0)
            {
                io = m_audioOutput->start();
            }
            
            if (m_audioOutput->state() == QAudio::StoppedState)
                return;

            if (bufferSize > SamplesPerFrame)
                return;
            
            int period = m_audioOutput->periodSize();
            int free = m_audioOutput->bytesFree();
            if (free > period)
            {
                bufferSize += period;
                io->write(m_buffer->data(), period);
            }
        }

        void NesApuChannel::setVolume(qreal volume)
        {
            if (m_audioOutput != 0)
            {
                m_audioOutput->setVolume(volume);
            }
        }

    }
}
