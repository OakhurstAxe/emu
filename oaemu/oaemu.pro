TEMPLATE = app

QT += widgets core gui gamepad

CONFIG += debug

SOURCES += main.cc
LIBS += -L"$$_PRO_FILE_PWD_/../oaemumemory/build" -loaemumemory 
LIBS += -L"$$_PRO_FILE_PWD_/../oaemucpu/build" -loaemucpu 
LIBS += -L"$$_PRO_FILE_PWD_/../oanesmapper/build" -loanesmapper
LIBS += -L"$$_PRO_FILE_PWD_/../oanes/build" -loanes
LIBS += -L"$$_PRO_FILE_PWD_/../oavcs/build" -loavcs

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build
