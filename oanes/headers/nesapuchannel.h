#ifndef _OA_NES_APUCHANNEL_H
#define _OA_NES_APUCHANNEL_H

#include <QIODevice>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>

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
            qint64 bytesAvailable() const override;
            void playSound(int inFrequency);
            void setVolume(qreal volume);
        private:
            int bufferSize = 0;
            QIODevice *io = 0;
            int frequency = 0;
            void generateData(const QAudioFormat &format, qint64 durationUs, int frequency);
            QAudioFormat m_format;
            QAudioDeviceInfo m_device;
            void createAudioOutput();
            QAudioOutput *m_audioOutput;
            qint64 m_pos;
            int currentBuffer=0;
            QByteArray *m_buffer;
        };

    }
}
#endif

