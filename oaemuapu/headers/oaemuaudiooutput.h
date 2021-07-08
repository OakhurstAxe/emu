#ifndef EMUAUDIOOUTPUT_H
#define EMUAUDIOOUTPUT_H

#include <math.h>

#include <QAudioOutput>
#include <QByteArray>
#include <QComboBox>
#include <QIODevice>
#include <QLabel>
#include <QMainWindow>
#include <QObject>
#include <QPushButton>
#include <QSlider>
#include <QTimer>

#include "../oaemumemory/headers/oanesmemory.h"

class EmuAudio : public QIODevice
{
    Q_OBJECT

public:
    EmuAudio(QObject *parent);
    ~EmuAudio();

    void start();
    void stop();

    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;
    qint64 bytesAvailable() const override;
    void playSound(qint64 durationUs, int sampleRate);
    NesMemory *memory;
    void executeTicks();
private:
    int frequency = 0;
    void generateData(const QAudioFormat &format, qint64 durationUs, int sampleRate);
    QAudioFormat m_format;
    QAudioDeviceInfo m_device;
    void createAudioOutput();
    QAudioOutput *m_audioOutput;
private:
    qint64 m_pos;
    QByteArray m_buffer;
};

#endif // AUDIOOUTPUT_H
