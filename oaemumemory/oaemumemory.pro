
TEMPLATE = lib

CONFIG += debug

SOURCES += basememory.cc memoryram.cc memoryrom.cc memoryramflagged.cc

INCLUDEPATH += ..

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build
