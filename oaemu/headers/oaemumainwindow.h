#ifndef EMUMAINWINDOW_H
#define EMUMAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QTimer>
#include <QImage>

#include "../oaemucpu/headers/oam6502.h"
#include "../oaemucpu/headers/oanesppu.h"
#include "../oaemuapu/headers/oaemuaudiooutput.h"
#include "../oaemuapu/headers/oaemuapu.h"

QT_BEGIN_NAMESPACE
class QGamepad;
QT_END_NAMESPACE

class EmuMainWindow : public QWidget
{
    Q_OBJECT
    
public:
    EmuMainWindow();
    ~EmuMainWindow();
    unsigned char leftController;
    unsigned char rightController;
public slots:
    void startNextFrame();
    void leftControllerLeftRight(double value);
    void leftControllerUpDown(double value);
    void leftControllerSelect(bool value);
    void leftControllerStart(bool value);
    void leftControllerA(bool value);
    void leftControllerB(bool value);
protected:
    void paintEvent(QPaintEvent *event);
    void readGamepad();
private:
    bool inFrame = false;
    QGamepad *m_gamepad;
    QImage *qImage2;
    
    QColor palette[64];
    
    void loadClicked();
    void loadDKong();
    QTimer *cpuTimer;
    M6502 cpu;
    NesPpu ppu;
    //EmuAudio *apu;
    EmuApu *apu;
    const static int SIZE = 16384 + 8192;
    char dKongFile[SIZE];
};

#endif // EMUMAINWINDOW_H
