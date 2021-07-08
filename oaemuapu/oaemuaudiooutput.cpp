#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QDebug>
#include <QVBoxLayout>
#include <qmath.h>
#include <qendian.h>

#include "headers/oaemuaudiooutput.h"

#define PUSH_MODE_LABEL "Enable push mode"
#define PULL_MODE_LABEL "Enable pull mode"
#define SUSPEND_LABEL   "Suspend playback"
#define RESUME_LABEL    "Resume playback"
#define VOLUME_LABEL    "Volume:"

const int DurationSeconds = 1;
const int ToneSampleRateHz = 600;
const int DataSampleRateHz = 44100;
const int BufferSize      = 32768;

EmuAudio::EmuAudio(QObject *parent) : QIODevice(parent)
    , m_pos(0)
    , m_device(QAudioDeviceInfo::defaultOutputDevice())
    , m_audioOutput(0)
{
}

EmuAudio::~EmuAudio()
{

}

void EmuAudio::executeTicks()
{
    int timer = 0;
    
    unsigned char byte = memory->cpuRead(0x4006);
    timer |= byte;
    byte = (memory->cpuRead(0x4007) & 0x07);
    timer |= byte << 8;
    if (timer < 8)
    {
        stop();
        return;
    }
    int newFrequency = 1789773/(16 * (timer + 1));
    if (newFrequency < 100)
    {
        stop();
        return;
    }
    if (frequency != newFrequency)
    {
        stop();
        frequency = newFrequency;
        playSound(50000, frequency);
    }
}

void EmuAudio::playSound(qint64 durationUs, int sampleRate)
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

void EmuAudio::start()
{
    
    m_audioOutput->start(this);
}

void EmuAudio::stop()
{
//    m_pos = 0;
  //  close();
    if (m_audioOutput != 0)
    {
        m_audioOutput->stop();
    }
}

void EmuAudio::generateData(const QAudioFormat &format, qint64 durationUs, int frequency)
{
    const int channelBytes = format.sampleSize() / 8;
    const int sampleBytes = channelBytes;

    qint64 length = (format.sampleRate() * (format.sampleSize() / 8))
                        * durationUs / 100000;

    if (length % 2 == 1)
        length--;
    
    Q_ASSERT(length % sampleBytes == 0);
    Q_UNUSED(sampleBytes) // suppress warning in release builds

    m_buffer.resize(length);
    unsigned char *ptr = reinterpret_cast<unsigned char *>(m_buffer.data());
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
}

qint64 EmuAudio::readData(char *data, qint64 len)
{
    qint64 total = 0;
    if (!m_buffer.isEmpty()) {
        while (len - total > 0) {
            const qint64 chunk = qMin((m_buffer.size() - m_pos), len - total);
            memcpy(data + total, m_buffer.constData() + m_pos, chunk);
            m_pos = (m_pos + chunk) % m_buffer.size();
            total += chunk;
        }
    }
    return total;
}

qint64 EmuAudio::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;
}

qint64 EmuAudio::bytesAvailable() const
{
    return m_buffer.size() + QIODevice::bytesAvailable();
}

void EmuAudio::createAudioOutput()
{
    open(QIODevice::ReadWrite);
    delete m_audioOutput;
    m_audioOutput = 0;
    m_audioOutput = new QAudioOutput(m_device, m_format, this);
    m_audioOutput->setVolume(qreal(0.5f));
    m_audioOutput->start(this);
    QAudio::State state = m_audioOutput->state();
    QAudio::Error error = m_audioOutput->error();
   
    //m_audioOutput->resume();
}
