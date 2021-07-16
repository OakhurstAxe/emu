
#include "headers/nesconsole.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <QMessageBox>

#include "headers/inesfile.h"

namespace oa
{
    namespace nes
    {
        NesConsole::NesConsole(NesMainWindow* nesMainWindow)
        {
            cpu_ = new R2A03(&nesMemory_);
            ppu_ = new NesPpu(&nesMemory_);
            apu_ = new Apu(&nesMemory_);
            
            nesMainWindow_ = nesMainWindow;
        }
        
        NesConsole::~NesConsole()
        {
            delete apu_;
            delete ppu_;
            delete cpu_;
        }
        
        void NesConsole::StartUp()
        {
            INesFile iNesFile;
            
            iNesFile.LoadFile("roms/Donkey_kong.nes");
            nesMemory_.LoadProgRom(iNesFile.progRomData_,0x4000);
            nesMemory_.LoadProgRom(iNesFile.charRomData_,0x2000);
            
            /*
            LoadDKong();
            unsigned char prgRom[0x4000];
            for (int i=0; i<0x4000; i++)
            {
                prgRom[i] = dKongFile[i+16];
            }
            nesMemory_.LoadProgRom(prgRom,0x4000);
            
            unsigned char charRom[0x2000];
            for (int i=0; i<0x2000; i++)
            {
                charRom[i] = dKongFile[i+16+0x4000];
            }
            nesMemory_.LoadCharRom(charRom,0x2000);
            */
            cpu_->Reset();

            
            connect(&cpuTimer_, SIGNAL(timeout()), SLOT(StartNextFrame()));
            cpuTimer_.setTimerType(Qt::PreciseTimer);
            cpuTimer_.setInterval(16);
            cpuTimer_.start();
        }
        
        void NesConsole::LoadDKong()
        {    
            std::ifstream inFile;
            inFile.open("roms/Donkey_kong.nes", std::ios::in | std::ios::binary | std::ios::ate);
            
            if (inFile.is_open())
            {
                inFile.seekg(0, std::ios::beg);
                inFile.read(dKongFile, SIZE);        
                inFile.close();
            }
            else
            {
                std::cerr << "Can't find Donky Kong file" << std::endl;
            }
        }
        
        void NesConsole::StartNextFrame()
        {
            try 
            {
                int ticks = 0;
                while (ticks < 49917)
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
//                    apu_.ExecuteTick();
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
