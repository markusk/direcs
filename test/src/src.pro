SOURCES += test.cpp \
    main.cpp \
    direcsSerial.cpp \
    interfaceAvr.cpp \
    circuit.cpp \
    laser.cpp \
    laserSickS300.cpp \
    laserThread.cpp
HEADERS += test.h \
    direcsSerial.h \
    interfaceAvr.h \
    circuit.h \
    laser.h \
    laserSickS300.h \
    laserThread.h
TEMPLATE = app
CONFIG += warn_on \
    thread \
    qt
TARGET = test
DESTDIR = ../bin
RESOURCES = application.qrc
FORMS += 
