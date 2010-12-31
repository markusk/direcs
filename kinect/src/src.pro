SOURCES += kinect.cpp \
    main.cpp
HEADERS += kinect.h
TEMPLATE = app
CONFIG += warn_on \
    thread \
    qt
TARGET = kinect
DESTDIR = ../bin
RESOURCES = application.qrc
FORMS += 
MOC_DIR = ../tmp
OBJECTS_DIR = ../tmp
