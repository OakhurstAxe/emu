
#include "headers/nesconsole.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <QMessageBox>
#include <QDebug>

#include "headers/inesfile.h"

#define TICKS_PER_FRAME 49917

namespace oa
{
    namespace nes
    {
        NesConsole::NesConsole(NesMainWindow* nesMainWindow)
        {
            cpuTimer_ = new QTimer();
            cpu_ = new R2A03(&nesMemory_);
            ppu_ = new NesPpu(&nesMemory_);
            apu_ = new NesApu(&nesMemory_);
            
            nesMainWindow_ = nesMainWindow;
        }
        
        NesConsole::~NesConsole()
        {
            delete cpuTimer_;
            delete apu_;
            delete ppu_;
            delete cpu_;
        }
        
        void NesConsole::StartUp()
        {
            INesFile iNesFile;
           
            //iNesFile.LoadFile("roms/Donkey_kong.nes");
            iNesFile.LoadFile("roms/Excitebike (E).nes");
            //iNesFile.LoadFile("roms/cpu.nes");
            
            nesMemory_.LoadProgRom(iNesFile.progRomData_, iNesFile.GetProgRomSize());
            nesMemory_.LoadCharRom(iNesFile.charRomData_, iNesFile.GetCharRomSize());

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
            //catch (std::exception &exception)
            //{
            //    qDebug() << "Error in frame" << exception.what();
            //    throw std::out_of_range(QString("Error in frame [%1]").arg(exception.what()).toLocal8Bit().data());
            //}  
        }
        
        void NesConsole::ReadGamepad()
        {
            if (nesMemory_.CpuWriteFlagged(0x4016))
            {
                nesMemory_.SetLeftController(nesMainWindow_->leftController);
            }
            if (nesMemory_.CpuWriteFlagged(0x4017))
            {
                nesMemory_.SetRightController(0);
            }
        }
    }
}
