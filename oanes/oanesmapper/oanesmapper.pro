
TEMPLATE = lib

QT += widgets

CONFIG += debug

SOURCES += nescartridge.cc
SOURCES += nescartridge000.cc nescartridge001.cc

LIBS += -L"$$_PRO_FILE_PWD_/../../oaemumemory/build" -loaemumemory 
LIBS += -L"$$_PRO_FILE_PWD_/../../oaemucpu/build" -loaemucpu 

INCLUDEPATH += ../..

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build


