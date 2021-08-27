
#include "headers/vcsconsole.h"
#include "headers/vcsfile.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <QMessageBox>
#include <QDebug>

#define TICKS_PER_FRAME 59736

namespace oa
{
    namespace vcs
    {
        VcsConsole::VcsConsole(VcsMainWindow* vcsMainWindow)
        {
            vcsMainWindow_ = vcsMainWindow;

            vcsTia_ = new VcsTia();
            ram_ = new emu::MemoryRam(0x80, "VCS Ram");
            vscRiot_ = new VcsRiot();
            vcsCartridge_ = new VcsCartridge();
            
            vcsMemory_ = new VcsMemory(vcsTia_, ram_, vscRiot_, vcsCartridge_);
            cpu_ = new M6507(vcsMemory_);
            cpuTimer_ = new QTimer();
        }
        
        VcsConsole::~VcsConsole()
        {
            delete cpuTimer_;
            delete vcsCartridge_;
            delete vscRiot_;
            delete ram_;
            delete vcsTia_;
            delete cpu_;
            delete vcsMemory_;
        }
       
        void VcsConsole::StartUp()
        {
            VcsFile vcsFile;
           
            vcsFile.LoadFile("vcsroms/ROMS/Adventure (1980) (Atari, Warren Robinett - Sears) (CX2613 - 49-75154) ~.bin");
            //vcsFile.LoadFile("vcsroms/ROMS/Combat - Tank-Plus (Tank) (1977) (Atari, Joe Decuir, Larry Kaplan, Steve Mayer, Larry Wagner - Sears) (CX2601 - 99801, 6-99801, 49-75101, 49-75124) ~.bin");

            vcsCartridge_->LoadData(vcsFile.GetProgRomData(), vcsFile.GetProgRomSize());

            cpu_->Reset();
            vscRiot_->Reset();
            vcsTia_->Reset();
            
            connect(cpuTimer_, SIGNAL(timeout()), SLOT(StartNextFrame()));
            cpuTimer_->setTimerType(Qt::PreciseTimer);
            cpuTimer_->setInterval(16);
            cpuTimer_->start();
        }
        
        void VcsConsole::StartNextFrame()
        {
            //qDebug() << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            {
                int ticks = 0;
                //apu_->ExecuteTick();
                while (ticks < TICKS_PER_FRAME)
                {
                    if ((ticks % 3) == 0)
                    {
                        cpu_->ExecuteTick(vcsTia_->IsCpuBlocked());
                        vscRiot_->ExecuteTick();
                    }
                    vcsTia_->ExecuteTick();
                    ReadGamepad();
                    ticks++;
                }
                vcsMainWindow_->DrawFrame(vcsTia_->GetScreen());
            }
        }
        
        void VcsConsole::ReadGamepad()
        {
        }

    }
}
