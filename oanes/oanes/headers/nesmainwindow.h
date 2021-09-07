#ifndef _OA_NES_MAINWINDOW_H
#define _OA_NES_MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QTimer>
#include <QImage>

#include "nespalette.h"
#include "r2a03.h"
#include "nesppu.h"
#include "nesapu.h"


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
            uint8_t leftController_ = 0;
            uint8_t rightController_ = 0;
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
            bool inFrame_ = false;
            QGamepad *m_gamepad_;
            QImage *qImage_;
        };

    }
}
#endif // EMUMAINWINDOW_H
