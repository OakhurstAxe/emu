TEMPLATE = app

QT += widgets core gui testlib gamepad multimedia

CONFIG += debug

SOURCES += main.cc
LIBS += -L"$$_PRO_FILE_PWD_/../oaemumemory/build" -loaemumemory 
LIBS += -L"$$_PRO_FILE_PWD_/../oaemucpu/build" -loaemucpu 
LIBS += -L"$$_PRO_FILE_PWD_/../oanes/build" -loanes

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build



