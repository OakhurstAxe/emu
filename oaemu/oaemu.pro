TEMPLATE = app

QT += widgets core gui testlib gamepad multimedia

CONFIG += debug

FORMS += oaemumainwindow.ui
HEADERS += headers/oaemumainwindow.h 
SOURCES += main.cpp oaemumainwindow.cpp
LIBS += -L"$$_PRO_FILE_PWD_/../oaemucpu/build" -loaemucpu -L"$$_PRO_FILE_PWD_/../oaemumemory/build" -loaemumemory 
LIBS += -L"$$_PRO_FILE_PWD_/../oaemuapu/build" -loaemuapu

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build



