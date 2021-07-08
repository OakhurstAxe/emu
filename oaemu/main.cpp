#include <QApplication>
#include <QMainWindow>

#include "headers/oaemumainwindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    EmuMainWindow emu;
    emu.resize(1024, 768);
    emu.show();
    
    return app.exec();
}
