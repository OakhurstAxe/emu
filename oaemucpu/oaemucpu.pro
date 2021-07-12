TEMPLATE = lib

CONFIG += debug c++17

HEADERS += headers/basecpu.h headers/m6502.h
SOURCES += basecpu.cc m6502.cc

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build
