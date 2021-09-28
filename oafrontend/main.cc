#include <QApplication>
#include <QMainWindow>

#include "headers/oaemusystemdata.h"
#include "headers/oaemumainwindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    //oa::emu::SystemData systemData;
    //systemData.LoadFile("/home/dmax/projects/oaemu/SystemData.txt");
    //systemData.SaveFile("/home/dmax/projects/oaemu/TestSystemData.txt");
    
    oa::emu::MainWindow widget;
    widget.show();
    
    return app.exec();
}
