
TEMPLATE = lib

QT += widgets core gui gamepad

CONFIG += debug

INCLUDEPATH += ..

HEADERS += headers/vcscartridge.h headers/vcsconsole.h headers/vcsmainwindow.h headers/vcsmemory.h 
HEADERS += headers/vcsfile.h headers/vcstia.h headers/m6507.h headers/vcspalette.h
HEADERS += headers/vcsriot.h headers/vcsinput.h
HEADERS += headers/vcsaudio.h headers/vcsaudiochannel.h
HEADERS += headers/vcsconsoletype.h headers/vcsparameters.h
HEADERS += headers/vcscartridge2k.h headers/vcscartridge4k.h headers/vcscartridgesc.h

SOURCES += vcscartridge.cc vcsconsole.cc vcsmainwindow.cc vcsmemory.cc 
SOURCES += vcsfile.cc vcstia.cc m6507.cc vcspalette.cc
SOURCES += vcsriot.cc vcsinput.cc
SOURCES += vcsaudio.cc vcsaudiochannel.cc
SOURCES += vcsconsoletype.cc vcsparameters.cc
SOURCES += vcscartridge2k.cc vcscartridge4k.cc vcscartridgesc.cc

LIBS += -L"$$_PRO_FILE_PWD_/../oaemumemory/build" -loaemumemory
LIBS += -L"$$_PRO_FILE_PWD_/../oaemucpu/build" -loaemucpu 
LIBS += -lportaudio

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build


