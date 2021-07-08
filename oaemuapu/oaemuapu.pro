TEMPLATE = lib

QT += widgets core gui testlib gamepad multimedia

CONFIG += debug

HEADERS += headers/oaemuaudiooutput.h headers/oaemuapu.h headers/oaemuapuchannel.h
SOURCES += oaemuaudiooutput.cpp oaemuapu.cpp oaemuapuchannel.cpp

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build




