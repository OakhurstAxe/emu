#include <QApplication>
#include <QMainWindow>

#include "headers/oaemusystemdata.h"
#include "headers/oaemumainwindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    
    oa::emu::MainWindow widget;
    widget.show();
    
    return app.exec();
}
