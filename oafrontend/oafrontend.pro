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

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build
