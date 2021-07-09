#ifndef EMUMAINWINDOW_H
#define EMUMAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QTimer>
#include <QImage>

#include "../oanes/headers/r2a03.h"
#include "../oanes/headers/nesppu.h"
#include "../oanes/headers/oaemuaudiooutput.h"
#include "../oanes/headers/nesapu.h"

QT_BEGIN_NAMESPACE
class QGamepad;
QT_END_NAMESPACE

namespace oa
{
    namespace nes
    {
    
        class NesMainWindow : public QWidget
        {
            Q_OBJECT
            
        public:
            NesMainWindow();
            ~NesMainWindow();
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
            oa::nes::R2A03 cpu;
            oa::nes::NesPpu ppu;
            //EmuAudio *apu;
            oa::nes::EmuApu *apu;
            const static int SIZE = 16384 + 8192;
            char dKongFile[SIZE];
        };

    }
}
#endif // EMUMAINWINDOW_H
