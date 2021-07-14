
TEMPLATE = lib

QT += widgets core gui testlib gamepad multimedia

CONFIG += debug c++17

FORMS += nesmainwindow.ui
HEADERS += headers/nesmemory.h headers/apu.h headers/nesapuchannel.h headers/oaemuaudiooutput.h headers/nesppu.h headers/nesmainwindow.h
HEADERS += headers/r2a03.h headers/nesconsole.h headers/nespalette.h
SOURCES += nesmemory.cc apu.cc nesapuchannel.cc oaemuaudiooutput.cpp nesppu.cc nesmainwindow.cc
SOURCES += r2a03.cc nesconsole.cc nespalette.cc
LIBS += -L"$$_PRO_FILE_PWD_/../oaemumemory/build" -loaemumemory -L"$$_PRO_FILE_PWD_/../oaemucpu/build" -loaemucpu

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build

