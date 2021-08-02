#include <QDebug>
#include <qendian.h>

#include <chrono>
#include <iostream>
#include <sys/time.h>
#include <ctime>


#include "headers/nesapuchannel.h"

namespace oa
{
    namespace nes
    {
        
        NesApuChannel::NesApuChannel() : 
              m_device(QAudioDeviceInfo::defaultOutputDevice())
            , m_audioOutput(0)
        {
            m_buffer = new QByteArray();
            m_buffer->resize(BufferSize);
        }

        NesApuChannel::~NesApuChannel()
        {
            delete m_buffer;
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

        void NesApuChannel::SetVolume(qreal volume)
        {
            if (m_audioOutput != 0)
            {
                m_audioOutput->setVolume(volume);
            }
        }
        
        void NesApuChannel::WriteAudioOutput()
        {
            qDebug() << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            
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
                m_audioOutput->setBufferSize(SamplesPerFrame);
                io = m_audioOutput->start();
            }
            
            if (m_audioOutput->state() == QAudio::StoppedState)
                return;

            if (bufferSize >= SamplesPerFrame)
                return;
            
            auto format = m_audioOutput->format();
            auto samples = format.sampleRate();
            auto size = format.sampleSize();
            auto bytes = format.bytesPerFrame();
            auto duration = format.bytesForDuration(15000);
            
            //int period = m_audioOutput->periodSize();
            int period = SamplesPerFrame;
            int free = m_audioOutput->bytesFree();
            if (free >= period)
            {
                bufferSize += period;
                auto written = io->write(m_buffer->data(), period);
            }
        }

    }
}
