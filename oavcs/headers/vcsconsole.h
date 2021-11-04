#ifndef _OA_VCS_CONSOLE_H
#define _OA_VCS_CONSOLE_H

#include <QObject>
#include <QMainWindow>
#include <QTimer>

#include "m6507.h"
#include "vcsaudio.h"
#include "vcsmemory.h"
#include "vcsparameters.h"
#include "vcsriot.h"
#include "vcstia.h"

namespace oa
{
    namespace vcs
    {
        class VcsConsole : public QWidget
        {
            Q_OBJECT
            
        public:
            VcsConsole(/*VcsMainWindow* vcsMainWindow,*/ VcsParameters* vcsParameters);
            virtual ~VcsConsole();
            void StartUp();
        public slots:
            void StartNextFrame();
        protected:
            void paintEvent(QPaintEvent *event);            
        private:
            VcsRiot vcsRiot_;
            VcsConsoleType vcsConsoleType_;
            VcsTia vcsTia_;
            VcsMemory vcsMemory_;
            VcsAudio vcsAudio_;
            M6507 cpu_;
            
            //VcsMainWindow *vcsMainWindow_;
            QTimer cpuTimer_;
            uint32_t ticksPerFrame_;
            
            QImage *qImage_;
        };
        
    }
}


#endif
