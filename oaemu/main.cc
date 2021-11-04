#include <QApplication>
#include <QMainWindow>

#include "oanes/oanes/headers/nesmainwindow.h"
#include "oavcs/headers/vcsconsole.h"
#include "oavcs/headers/vcsconsoletype.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    oa::nes::NesMainWindow nes;
    nes.resize(1024, 768);
    nes.show();
    
    //oa::vcs::VcdConsole vcs(oa::vcs::ConsoleType::NTSC);
    //vcs.show();

    return app.exec();
}
