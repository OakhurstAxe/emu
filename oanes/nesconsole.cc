
#include "headers/nesconsole.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <QMessageBox>
#include <QDebug>

#include "headers/inesfile.h"
#include "../oanesmapper/headers/nescartridge.h"

#define TICKS_PER_FRAME 49917

namespace oa
{
    namespace nes
    {
        NesConsole::NesConsole(NesMainWindow* nesMainWindow)
        {
            nesMemory_ = new NesMemory();
            cpuTimer_ = new QTimer();
            cpu_ = new R2A03(nesMemory_);
            ppu_ = new NesPpu(nesMemory_);
            apu_ = new NesApu(nesMemory_);
            
            nesMainWindow_ = nesMainWindow;
        }
        
        NesConsole::~NesConsole()
        {
            delete cpuTimer_;
            delete apu_;
            delete ppu_;
            delete cpu_;
            if (nesCartridge_ != 0)
            {
                delete nesCartridge_;
            }
            delete nesMemory_;
        }
        
        void NesConsole::StartUp()
        {
            INesFile iNesFile;
           
            //iNesFile.LoadFile("roms/Donkey_kong.nes");
            //iNesFile.LoadFile("roms/Excitebike (E).nes");
            iNesFile.LoadFile("roms/Ice Climber (U).nes");
            //iNesFile.LoadFile("roms/cpu.nes");
            
            nesCartridge_ = NesCartridge::GetCartridge(iNesFile.GetMemoryMapper());
            nesCartridge_->LoadProgRom(iNesFile.GetProgRomData(), iNesFile.GetProgRomSize());
            nesCartridge_->LoadCharRom(iNesFile.GetCharRomData(), iNesFile.GetCharRomSize());
            nesMemory_->SetCartridge(nesCartridge_);

            cpu_->Reset();
            
            connect(cpuTimer_, SIGNAL(timeout()), SLOT(StartNextFrame()));
            cpuTimer_->setTimerType(Qt::PreciseTimer);
            cpuTimer_->setInterval(16);
            cpuTimer_->start();
        }
        
        void NesConsole::StartNextFrame()
        {
            //qDebug() << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            //try 
            {
                int ticks = 0;
                apu_->ExecuteTick();
                while (ticks < TICKS_PER_FRAME)
                {
                    if ((ticks % 3) == 0)
                    {
                        cpu_->ExecuteTick();
                    }
                    ppu_->ExecuteTick();
                    if (ppu_->IsNmiSet())
                    {
                        cpu_->SetNmi();
                        ppu_->ResetNmi();
                    }
                    ReadGamepad();
                    ticks++;
                }
                nesMainWindow_->DrawFrame(ppu_->GetScreen());
            }
        }
        
        void NesConsole::ReadGamepad()
        {
            if (nesMemory_->CpuWriteFlagged(0x4016))
            {
                nesMemory_->SetLeftController(nesMainWindow_->leftController_);
            }
            if (nesMemory_->CpuWriteFlagged(0x4017))
            {
                nesMemory_->SetRightController(nesMainWindow_->rightController_);
            }
        }
    }
}
