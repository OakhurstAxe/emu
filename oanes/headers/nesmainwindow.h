#ifndef _OA_NES_MAINWINDOW_H
#define _OA_NES_MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QTimer>
#include <QImage>

#include "../headers/nespalette.h"
#include "../oanes/headers/r2a03.h"
#include "../oanes/headers/nesppu.h"
#include "../oanes/headers/oaemuaudiooutput.h"
#include "../oanes/headers/apu.h"


QT_BEGIN_NAMESPACE
class QGamepad;
QT_END_NAMESPACE

namespace oa
{
    namespace nes
    {

        class NesConsole;
        
        class NesMainWindow : public QWidget
        {
            Q_OBJECT
            
        public:
            NesMainWindow();
            ~NesMainWindow();
            void DrawFrame(uint8_t *screen);
            unsigned char leftController = 0;
            unsigned char rightController = 0;
        public slots:
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
            NesPalette nesPalette_;
            NesConsole *nesConsole_;
            uint8_t *screen_;
            bool inFrame = false;
            QGamepad *m_gamepad;
            QImage *qImage;
        };

    }
}
#endif // EMUMAINWINDOW_H
