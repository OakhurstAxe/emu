TEMPLATE = app

QT += widgets core gui testlib gamepad multimedia

CONFIG += debug

FORMS += emumainwindow.ui
HEADERS += headers/emumainwindow.h 
SOURCES += main.cpp emumainwindow.cpp
LIBS += -L"$$_PRO_FILE_PWD_/../emucpu/build" -lemucpu -L"$$_PRO_FILE_PWD_/../emumemory/build" -lemumemory -L"$$_PRO_FILE_PWD_/../emuapu/build" -lemuapu

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build



