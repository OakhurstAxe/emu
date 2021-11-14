
TEMPLATE = lib

QT += widgets core gui gamepad

CONFIG += debug

INCLUDEPATH += ..

HEADERS += headers/vcscartridge.h headers/vcsconsole.h headers/vcsmemory.h 
HEADERS += headers/vcstia.h headers/m6507.h headers/vcspalette.h
HEADERS += headers/vcsriot.h
HEADERS += headers/vcsaudio.h headers/vcsaudiochannel.h
HEADERS += headers/vcsconsoletype.h headers/vcsparameters.h
HEADERS += headers/vcscartridge2k.h headers/vcscartridge4k.h headers/vcscartridgee7.h
HEADERS += headers/vcscartridgef6.h headers/vcscartridgef8.h headers/vcscartridgee0.h
HEADERS += headers/vcscartridgefe.h

SOURCES += vcscartridge.cc vcsconsole.cc vcsmemory.cc 
SOURCES += vcstia.cc m6507.cc vcspalette.cc
SOURCES += vcsriot.cc
SOURCES += vcsaudio.cc vcsaudiochannel.cc
SOURCES += vcsconsoletype.cc vcsparameters.cc
SOURCES += vcscartridge2k.cc vcscartridge4k.cc vcscartridgee7.cc
SOURCES += vcscartridgef6.cc vcscartridgef8.cc vcscartridgee0.cc
SOURCES += vcscartridgefe.cc

LIBS += -L"$$_PRO_FILE_PWD_/../oaemumemory/build" -loaemumemory
LIBS += -L"$$_PRO_FILE_PWD_/../oaemucpu/build" -loaemucpu 
LIBS += -lportaudio

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build


