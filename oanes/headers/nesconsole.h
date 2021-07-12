#ifndef _OA_NES_CONSOLE
#define _OA_NES_CONSOLE

#include <memory>
#include <QTimer>

#include "../oaemumemory/headers/memoryram.h"
#include "../oaemumemory/headers/memoryrom.h"
#include "headers/r2a03.h"
#include "headers/ppu.h"
#include "headers/apu.h"
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
            NesMainWindow* nesMainWindow_;
            NesMemory nesMemory_;
            QTimer cpuTimer_;
            R2A03 *cpu_;
            Ppu ppu_;
            Apu apu_;
            const static int SIZE = 16384 + 8192;
            char dKongFile[SIZE];
        };
        
    }
}


#endif

