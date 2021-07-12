
#include "headers/nesconsole.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <QMessageBox>

namespace oa
{
    namespace nes
    {
        NesConsole::NesConsole(NesMainWindow* nesMainWindow)
        {
            cpu_ = new R2A03(&nesMemory_);
            ppu_.memory = &nesMemory_;
            ppu_.cpu = cpu_;
            apu_.memory = &nesMemory_;
            
            nesMainWindow_ = nesMainWindow;
        }
        
        NesConsole::~NesConsole()
        {
            delete cpu_;
        }
        
        void NesConsole::StartUp()
        {
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
                    ppu_.ExecuteTick();
                    apu_.ExecuteTick();
                    ReadGamepad();
                    ticks++;
                }
                nesMainWindow_->DrawFrame(ppu_.screen);
            }
            catch (std::exception &exception)
            {
                QMessageBox::information(0, QString("Error in frame"), QString(exception.what()), QMessageBox::Ok);
            }  
        }
        
        void NesConsole::ReadGamepad()
        {
            if (nesMemory_.loadController_ == 1)
            {
                nesMemory_.loadController_ = 0;
                nesMemory_.leftController_ = nesMainWindow_->leftController;
            }
        }
    }
}
