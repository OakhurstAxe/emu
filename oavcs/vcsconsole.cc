
#include "headers/vcsconsole.h"
#include "headers/vcsfile.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <QMessageBox>
#include <QDebug>

#define TICKS_PER_SECOND 3584160

namespace oa
{
    namespace vcs
    {
        VcsConsole::VcsConsole(VcsMainWindow* vcsMainWindow, VcsConsoleType* vcsConsoleType) :
            vcsTia_(vcsConsoleType), 
            ram_(0x80, "VCS Ram"),
            vcsMemory_(&vcsTia_, &ram_, &vscRiot_, &vcsCartridge_),
            vcsAudio_(&vcsMemory_),
            cpu_(&vcsMemory_)
        {
            vcsMainWindow_ = vcsMainWindow;
            vcsConsoleType_ = vcsConsoleType;
            ticksPerFrame_ = TICKS_PER_SECOND / vcsConsoleType_->GetFramesPerSecond();
            
            vcsMainWindow_->SetScreen(vcsTia_.GetScreen());
        }
        
        VcsConsole::~VcsConsole()
        {
            int x = 10;
        }
       
        void VcsConsole::StartUp(uint8_t* cartData, uint cartSize)
        {
            vcsCartridge_.LoadData(cartData, cartSize);

            cpu_.Reset();
            vscRiot_.Reset();
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
                vscRiot_.ExecuteTick();
                vcsTia_.ExecuteTick();
                if ((ticks % 3) == 0)
                {
                    cpu_.ExecuteTick(vcsTia_.IsCpuBlocked());
                }
                ticks++;
            }
            vcsMainWindow_->repaint();
        }
        
        void VcsConsole::ReadInput()
        {
            vcsMemory_.CpuWrite(0x280, vcsInput_.GetSwchaReg());
            vcsMemory_.CpuWrite(0x281, vcsInput_.GetSwcntReg());
            vcsMemory_.CpuWrite(0x282, vcsInput_.GetSwchbReg());
            vcsMemory_.CpuWrite(0x38, vcsInput_.GetInpt0Reg());
            vcsMemory_.CpuWrite(0x39, vcsInput_.GetInpt1Reg());
            vcsMemory_.CpuWrite(0x3A, vcsInput_.GetInpt2Reg());
            vcsMemory_.CpuWrite(0x3B, vcsInput_.GetInpt3Reg());
            vcsMemory_.CpuWrite(0x3C, vcsInput_.GetInpt4Reg());
            vcsMemory_.CpuWrite(0x3D, vcsInput_.GetInpt5Reg());
        }
    }
}
