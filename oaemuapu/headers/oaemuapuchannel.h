#ifndef _EMUAPUCHANNEL_H
#define _EMUAPUCHANNEL_H

#include <QIODevice>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>

class EmuApuChannel : public QIODevice
{
    Q_OBJECT

public:
    EmuApuChannel(QObject *parent);
    ~EmuApuChannel();

    void start();
    void stop();

    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;
    qint64 bytesAvailable() const override;
    void playSound(qint64 durationUs, int sampleRate);
    void setVolume(qreal volume);
private:
    int frequency = 0;
    void generateData(const QAudioFormat &format, qint64 durationUs, int frequency);
    QAudioFormat m_format;
    QAudioDeviceInfo m_device;
    void createAudioOutput();
    QAudioOutput *m_audioOutput;
    qint64 m_pos;
    int currentBuffer=0;
    QByteArray m_buffer[2];
};

#endif

