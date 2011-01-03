SOURCES += kinect.cpp \
    main.cpp \
    QKinect.cpp
HEADERS += kinect.h \
    QKinect.h
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
INCLUDEPATH += /usr/include /usr/include/libusb-1.0 /opt/local/include
LIBS += /usr/lib/libfreenect.so
