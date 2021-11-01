
#include "headers/vcsconsole.h"
#include "headers/vcsfile.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <QMessageBox>
#include <QDebug>

namespace oa
{
    namespace vcs
    {
        VcsConsole::VcsConsole(VcsMainWindow* vcsMainWindow, VcsParameters* vcsParameters) :
            vcsRiot_(&vcsInput_),
            vcsConsoleType_(vcsParameters->GetConsoleType()),
            vcsTia_(&vcsConsoleType_),
            vcsMemory_(&vcsTia_, &vcsRiot_, vcsParameters),
            vcsAudio_(&vcsTia_),
            cpu_(&vcsMemory_)
        {
            vcsMainWindow_ = vcsMainWindow;
            ticksPerFrame_ = vcsConsoleType_.TicksPerSecond() / vcsConsoleType_.GetFramesPerSecond();
            
            vcsMainWindow_->SetScreen(vcsTia_.GetScreen());
        }
        
        VcsConsole::~VcsConsole()
        {
        }
       
        void VcsConsole::StartUp()
        {
            cpu_.Reset();
            vcsRiot_.Reset();
            vcsTia_.Reset();

            connect(&cpuTimer_, SIGNAL(timeout()), SLOT(StartNextFrame()));
            cpuTimer_.setTimerType(Qt::PreciseTimer);
            cpuTimer_.setInterval(16);
            cpuTimer_.start();
        }
        
        VcsInput *VcsConsole::GetVcsInput()
        {
            return &vcsInput_;
        }
        
        void VcsConsole::StartNextFrame()
        {
            //qDebug() << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            ReadInput();
            uint32_t ticks = 0;
            vcsAudio_.ExecuteTick();
            while (ticks < ticksPerFrame_)
            {
                vcsRiot_.ExecuteTick();
                if (vcsTia_.IsCycleZero())
                {
                    while (ticks % 3 != 0)
                    {
                        ticks ++;
                    }
                }
                if ((ticks % 3) == 0 && !vcsTia_.IsCpuBlocked())
                {
                    cpu_.ExecuteTick();
                }
                vcsTia_.ExecuteTick();
                if (vcsTia_.Repaint())
                {
                    vcsMainWindow_->repaint();
                }
                ticks++;
            }
        }
        
        void VcsConsole::ReadInput()
        {
            //vcsMemory_.CpuWrite(0x280, vcsInput_.GetSwchaReg());
            //vcsMemory_.CpuWrite(0x281, vcsInput_.GetSwcntReg());
            //vcsMemory_.CpuWrite(0x282, vcsInput_.GetSwchbReg());
            vcsMemory_.CpuWrite(0x38, vcsInput_.GetInpt0Reg());
            vcsMemory_.CpuWrite(0x39, vcsInput_.GetInpt1Reg());
            vcsMemory_.CpuWrite(0x3A, vcsInput_.GetInpt2Reg());
            vcsMemory_.CpuWrite(0x3B, vcsInput_.GetInpt3Reg());
            vcsMemory_.CpuWrite(0x3C, vcsInput_.GetInpt4Reg());
            vcsMemory_.CpuWrite(0x3D, vcsInput_.GetInpt5Reg());
        }
    }
}
