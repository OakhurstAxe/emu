#ifndef _OA_NES_CONSOLE
#define _OA_NES_CONSOLE

#include <memory>
#include <QTimer>

#include "../oaemumemory/headers/memoryram.h"
#include "../oaemumemory/headers/memoryrom.h"
#include "../oanesmapper/headers/nescartridge.h"
#include "headers/r2a03.h"
#include "headers/nesppu.h"
#include "headers/nesapu.h"
#include "headers/nesmainwindow.h"

namespace oa
{
    namespace nes
    {
 
        class NesConsole : public QObject
        {
            Q_OBJECT
            
        public:
            NesConsole(NesMainWindow* nesMainWindow);
            virtual ~NesConsole();
            void StartUp();
        public slots:
            void StartNextFrame();
        private:
            void ReadGamepad();
            void LoadDKong();
            
            NesMainWindow *nesMainWindow_;
            NesMemory *nesMemory_;
            NesCartridge *nesCartridge_;
            QTimer *cpuTimer_;
            R2A03 *cpu_;
            NesPpu *ppu_;
            NesApu *apu_;
            const static int SIZE = 16384 + 8192;
            char dKongFile[SIZE];
        };
        
    }
}


#endif

