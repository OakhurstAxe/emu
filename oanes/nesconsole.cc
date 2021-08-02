
#include "headers/nesconsole.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <QMessageBox>

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
            apu_ = new Apu(&nesMemory_);
            
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
           
            iNesFile.LoadFile("roms/Donkey_kong.nes");
            nesMemory_.LoadProgRom(iNesFile.progRomData_, 0x4000);
            nesMemory_.LoadCharRom(iNesFile.charRomData_, 0x2000);

            nesMemory_.LoadProgRom(iNesFile.GetProgRomData(), iNesFile.GetProgRomSize());
            nesMemory_.LoadCharRom(iNesFile.GetCharRomData(), iNesFile.GetCharRomSize());
            
            cpu_->Reset();
            
            connect(cpuTimer_, SIGNAL(timeout()), SLOT(StartNextFrame()));
            cpuTimer_->setTimerType(Qt::PreciseTimer);
            cpuTimer_->setInterval(13);
            cpuTimer_->start();
        }
        
        void NesConsole::StartNextFrame()
        {
            try 
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
            catch (std::exception &exception)
            {
                QMessageBox::information(0, QString("Error in frame"), QString(exception.what()), QMessageBox::Ok);
            }  
        }
        
        void NesConsole::ReadGamepad()
        {
            if (nesMemory_.IsLoadController())
            {
                nesMemory_.SetLeftController(nesMainWindow_->leftController);
            }
        }
    }
}
