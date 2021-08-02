#ifndef _OA_NES_APUCHANNEL_H
#define _OA_NES_APUCHANNEL_H

#include <QIODevice>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>

const int DataSampleRateHz = 44100;
const int BufferSize       = 66100;
const int SamplesPerFrame  = (661*2);//1470;
const int Period           = 3528;

namespace oa
{
    namespace nes
    {
        
        class NesApuChannel : public QIODevice
        {
            Q_OBJECT

        public:
            NesApuChannel();
            virtual ~NesApuChannel();

            qint64 readData(char *data, qint64 maxlen) override;
            qint64 writeData(const char *data, qint64 len) override;
            virtual void PlaySound(uint8_t register1, uint8_t register2, uint8_t register3, uint8_t register4) = 0;
            void SetVolume(qreal volume);
        protected:
            bool isrunning = false;
            void WriteAudioOutput();
            QAudioFormat m_format;
            QAudioDeviceInfo m_device;
            QAudioOutput *m_audioOutput;
            int bufferSize = 0;
            QIODevice *io = 0;
            QByteArray *m_buffer;
            qreal lastValue = 0.0;
        };

    }
}
#endif

