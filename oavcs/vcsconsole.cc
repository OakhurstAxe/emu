
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
            vcsInput_ = new VcsInput();
            
            vcsMemory_ = new VcsMemory(vcsTia_, ram_, vscRiot_, vcsCartridge_);
            vcsAudio_ = new VcsAudio(vcsMemory_);
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
            delete vcsAudio_;
            delete vcsMemory_;
            delete vcsInput_;
        }
       
        void VcsConsole::StartUp()
        {
            VcsFile vcsFile;
           
            //vcsFile.LoadFile("vcsroms/ROMS/Adventure (1980) (Atari, Warren Robinett - Sears) (CX2613 - 49-75154) ~.bin");
            vcsFile.LoadFile("vcsroms/ROMS/Combat - Tank-Plus (Tank) (1977) (Atari, Joe Decuir, Larry Kaplan, Steve Mayer, Larry Wagner - Sears) (CX2601 - 99801, 6-99801, 49-75101, 49-75124) ~.bin");

            vcsCartridge_->LoadData(vcsFile.GetProgRomData(), vcsFile.GetProgRomSize());

            cpu_->Reset();
            vscRiot_->Reset();
            vcsTia_->Reset();
            
            connect(cpuTimer_, SIGNAL(timeout()), SLOT(StartNextFrame()));
            cpuTimer_->setTimerType(Qt::PreciseTimer);
            cpuTimer_->setInterval(16);
            cpuTimer_->start();
        }
        
        VcsInput *VcsConsole::GetVcsInput()
        {
            return vcsInput_;
        }
        
        void VcsConsole::StartNextFrame()
        {
            //qDebug() << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            ReadInput();
            int ticks = 0;
            vcsAudio_->ExecuteTick();
            while (ticks < TICKS_PER_FRAME)
            {
                vscRiot_->ExecuteTick();
                vcsTia_->ExecuteTick();
                if ((ticks % 3) == 0)
                {
                    cpu_->ExecuteTick(vcsTia_->IsCpuBlocked());
                }
                ticks++;
            }
            vcsMainWindow_->DrawFrame(vcsTia_->GetScreen());
        }
        
        void VcsConsole::ReadInput()
        {
            vcsMemory_->CpuWrite(0x280, vcsInput_->GetSwchaReg());
            vcsMemory_->CpuWrite(0x281, vcsInput_->GetSwcntReg());
            vcsMemory_->CpuWrite(0x282, vcsInput_->GetSwchbReg());
            vcsMemory_->CpuWrite(0x38, vcsInput_->GetInpt0Reg());
            vcsMemory_->CpuWrite(0x39, vcsInput_->GetInpt1Reg());
            vcsMemory_->CpuWrite(0x3A, vcsInput_->GetInpt2Reg());
            vcsMemory_->CpuWrite(0x3B, vcsInput_->GetInpt3Reg());
            vcsMemory_->CpuWrite(0x3C, vcsInput_->GetInpt4Reg());
            vcsMemory_->CpuWrite(0x3D, vcsInput_->GetInpt5Reg());
        }
    }
}
