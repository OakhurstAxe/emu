
TEMPLATE = lib

QT += widgets core gui testlib gamepad multimedia

CONFIG += debug c++17

FORMS += nesmainwindow.ui
HEADERS += headers/nesmemory.h headers/apu.h headers/nesapuchannel.h headers/oaemuaudiooutput.h headers/nesppu.h headers/nesmainwindow.h
HEADERS += headers/r2a03.h headers/nesconsole.h headers/nespalette.h headers/inesfile.h
HEADERS += headers/nesapupulsechannel.h headers/nesaputrianglechannel.h headers/nesapunoisechannel.h
SOURCES += nesmemory.cc apu.cc oaemuaudiooutput.cpp nesppu.cc nesmainwindow.cc
SOURCES += r2a03.cc nesconsole.cc nespalette.cc inesfile.cc
SOURCES += nesapuchannel.cc nesapupulsechannel.cc nesaputrianglechannel.cc nesapunoisechannel.cc
LIBS += -L"$$_PRO_FILE_PWD_/../oaemumemory/build" -loaemumemory -L"$$_PRO_FILE_PWD_/../oaemucpu/build" -loaemucpu

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build

