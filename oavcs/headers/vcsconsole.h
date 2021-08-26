#ifndef _OA_VCS_CONSOLE
#define _OA_VCS_CONSOLE

#include <memory>
#include <QTimer>

#include "../oaemumemory/headers/memoryram.h"
#include "../oaemumemory/headers/memoryrom.h"
#include "../oanesmapper/headers/nescartridge.h"
#include "../oaemucpu/headers/m6502.h"
#include "headers/vcsmainwindow.h"
#include "headers/vcsmemory.h"
#include "headers/vcscartridge.h"
#include "headers/vcstia.h"
#include "headers/m6507.h"
#include "headers/vcsriot.h"

namespace oa
{
    namespace vcs
    {
 
        class VcsConsole : public QObject
        {
            Q_OBJECT
            
        public:
            VcsConsole(VcsMainWindow* vcsMainWindow);
            virtual ~VcsConsole();
            void StartUp();
        public slots:
            void StartNextFrame();
        private:
            void ReadGamepad();
            
            VcsTia *vcsTia_;
            emu::MemoryRam *ram_;
            VcsRiot *vscRiot_;
            VcsCartridge *vcsCartridge_;
            M6507 *cpu_;
            VcsMemory *vcsMemory_;
            
            VcsMainWindow *vcsMainWindow_;
            QTimer *cpuTimer_;
        };
        
    }
}


#endif


