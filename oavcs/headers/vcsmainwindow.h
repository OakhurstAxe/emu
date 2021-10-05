
#ifndef _OA_VCS_MAINWINDOW_H
#define _OA_VCS_MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QTimer>
#include <QImage>
#include <QtGamepad/QGamepad>

#include "vcspalette.h"
#include "vcsinput.h"
#include "vcsconsoletype.h"
#include "vcsparameters.h"
#include "vcscartridge.h"

namespace oa
{
    namespace vcs
    {
        class VcsConsole;
        
        class VcsMainWindow : public QWidget
        {
            Q_OBJECT
            
        public:
            VcsMainWindow(VcsParameters vcsParameters);
            virtual ~VcsMainWindow();
            void SetScreen(uint8_t *screen);
        public slots:
            void LeftControllerA(bool value);
            void LeftControllerReset(bool value);
            void LeftControllerSelect(bool value);
            void LeftControllerUpDown(double value);
            void LeftControllerLeftRight(double value);
        protected:
            void paintEvent(QPaintEvent *event);
            void ReadGamepad();
        private:
            VcsConsoleType vcsConsoleType_;
            VcsPalette vcsPalette_;
            VcsConsole *vcsConsole_;
            uint8_t *screen_;
            bool inFrame_ = false;
            QGamepad m_gamepad_;
            QImage *qImage_;
            VcsInput *vcsInput;
            VcsCartridge *vcsCartridge_;
            
        };

    }
}
#endif // EMUMAINWINDOW_H
