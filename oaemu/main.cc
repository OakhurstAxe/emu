#include <QApplication>
#include <QMainWindow>

#include "../oanes/headers/nesmainwindow.h"
#include "../oavcs/headers/vcsmainwindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    //oa::nes::NesMainWindow nes;
    //nes.resize(1024, 768);
    //nes.show();
    
    oa::vcs::VcsMainWindow vcs;
    vcs.resize(640,768);
    vcs.show();

    return app.exec();
}
