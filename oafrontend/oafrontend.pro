TEMPLATE = app

QT += widgets
QT += xml

FORMS += oaemumainwindow.ui

HEADERS += headers/oaemuvcsgame.h headers/oaemuvcssystem.h headers/oaemuscreentypes.h
HEADERS += headers/oaemusystemdata.h headers/oaemumainwindow.h

SOURCES += main.cc
SOURCES += oaemuvcsgame.cc oaemuvcssystem.cc oaemuscreentypes.cc
SOURCES += oaemusystemdata.cc oaemumainwindow.cc

LIBS += -lminizip
LIBS += -L"$$_PRO_FILE_PWD_/../oaemumemory/build" -loaemumemory 
LIBS += -L"$$_PRO_FILE_PWD_/../oaemucpu/build" -loaemucpu 
LIBS += -L"$$_PRO_FILE_PWD_/../oavcs/build" -loavcs

INCLUDEPATH += ..

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build
