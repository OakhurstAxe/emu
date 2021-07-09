#include <QApplication>
#include <QMainWindow>

#include "../oanes/headers/nesmainwindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    oa::nes::NesMainWindow nes;
    nes.resize(1024, 768);
    nes.show();
    
    return app.exec();
}
