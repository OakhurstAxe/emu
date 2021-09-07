#ifndef _OA_VCS_CONSOLE
#define _OA_VCS_CONSOLE

#include <memory>
#include <QTimer>

#include "oaemumemory/headers/memoryram.h"
#include "oaemumemory/headers/memoryrom.h"
#include "oaemucpu/headers/m6502.h"
#include "vcsmainwindow.h"
#include "vcsmemory.h"
#include "vcscartridge.h"
#include "vcstia.h"
#include "m6507.h"
#include "vcsriot.h"
#include "vcsinput.h"
#include "vcsaudio.h"

namespace oa
{
    namespace vcs
    {
        class VcsMainWindow; 
 
        class VcsConsole : public QObject
        {
            Q_OBJECT
            
        public:
            VcsConsole(VcsMainWindow* vcsMainWindow, VcsConsoleType* vcsConsoleType);
            virtual ~VcsConsole();
            void StartUp();
            VcsInput *GetVcsInput();
        public slots:
            void StartNextFrame();
        private:
            void ReadInput();
            
            VcsConsoleType *vcsConsoleType_;
            VcsTia *vcsTia_;
            emu::MemoryRam *ram_;
            VcsRiot *vscRiot_;
            VcsCartridge *vcsCartridge_;
            M6507 *cpu_;
            VcsMemory *vcsMemory_;
            VcsInput *vcsInput_;
            VcsAudio *vcsAudio_;
            
            VcsMainWindow *vcsMainWindow_;
            QTimer *cpuTimer_;
            uint32_t ticksPerFrame_;
        };
        
    }
}


#endif


