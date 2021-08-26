
#ifndef _OA_VCS_MAINWINDOW_H
#define _OA_VCS_MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QTimer>
#include <QImage>

#include "vcspalette.h"

QT_BEGIN_NAMESPACE
class QGamepad;
QT_END_NAMESPACE

namespace oa
{
    namespace vcs
    {

        class VcsConsole;
        
        class VcsMainWindow : public QWidget
        {
            Q_OBJECT
            
        public:
            VcsMainWindow();
            ~VcsMainWindow();
            void DrawFrame(uint8_t *screen);
        public slots:
            void LeftControllerA(bool value);
            void LeftControllerUpDown(double value);
            void LeftControllerLeftRight(double value);
        protected:
            void paintEvent(QPaintEvent *event);
            void ReadGamepad();
        private:
            VcsPalette vcsPalette_;
            uint8_t leftController_ = 0;
            uint8_t rightController_ = 0;
            VcsConsole *vcsConsole_;
            uint8_t *screen_;
            bool inFrame_ = false;
            QGamepad *m_gamepad_;
            QImage *qImage_;
        };

    }
}
#endif // EMUMAINWINDOW_H
