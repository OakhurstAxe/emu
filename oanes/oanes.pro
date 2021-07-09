
TEMPLATE = lib

QT += widgets core gui testlib gamepad multimedia

CONFIG += debug

FORMS += nesmainwindow.ui
HEADERS += headers/nesmemory.h headers/nesapu.h headers/nesapuchannel.h headers/oaemuaudiooutput.h headers/nesppu.h headers/nesmainwindow.h
HEADERS += headers/r2a03.h
SOURCES += nesmemory.cc nesapu.cc nesapuchannel.cc oaemuaudiooutput.cpp nesppu.cc nesmainwindow.cc
SOURCES += r2a03.cc
LIBS += -L"$$_PRO_FILE_PWD_/../oaemumemory/build" -loaemumemory -L"$$_PRO_FILE_PWD_/../oaemucpu/build" -loaemucpu

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build

