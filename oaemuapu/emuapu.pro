TEMPLATE = lib

QT += widgets core gui testlib gamepad multimedia

CONFIG += debug

HEADERS += headers/emuaudiooutput.h headers/emuapu.h headers/emuapuchannel.h
SOURCES += emuaudiooutput.cpp emuapu.cpp emuapuchannel.cpp

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build




