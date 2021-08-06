
TEMPLATE = lib

QT += widgets core gui testlib gamepad multimedia

CONFIG += debug c++17

FORMS += nesmainwindow.ui

HEADERS += headers/nesmemory.h headers/r2a03.h headers/nesapu.h headers/nesppu.h headers/nesmainwindow.h
HEADERS += headers/nesconsole.h headers/nespalette.h headers/inesfile.h
HEADERS += headers/nesapuchannel.h headers/nesapupulsechannel.h headers/nesaputrianglechannel.h headers/nesapunoisechannel.h

SOURCES += nesmemory.cc r2a03.cc nesapu.cc nesppu.cc nesmainwindow.cc
SOURCES += nesconsole.cc nespalette.cc inesfile.cc
SOURCES += nesapuchannel.cc nesapupulsechannel.cc nesaputrianglechannel.cc nesapunoisechannel.cc

LIBS += -L"$$_PRO_FILE_PWD_/../oaemumemory/build" -loaemumemory -L"$$_PRO_FILE_PWD_/../oaemucpu/build" -loaemucpu 
LIBS += -lportaudio

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build

