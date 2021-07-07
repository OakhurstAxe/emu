#include <QApplication>
#include <QMainWindow>

#include "headers/emumainwindow.h"
#include "../emuapu/headers/emuaudiooutput.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    EmuMainWindow emu;
    emu.resize(1024, 768);
    emu.show();
    
    //AudioTest apu;
    //apu.resize(200,100);
    //apu.show();
    
    return app.exec();
}
