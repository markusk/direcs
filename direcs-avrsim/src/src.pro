DESTDIR = .

TARGET = ../bin/direcs-avrsim

MOC_DIR = ../tmp

OBJECTS_DIR = ../tmp

RESOURCES = ../direcsAvrsim.qrc

CONFIG += debug \
	qt

TEMPLATE = app

unix {
    HEADERS +=	direcsSerial.h \
				gui.h \
				direcs-avrsim.h \
	interfaceAvr.h

    SOURCES +=	direcsSerial.cpp \
				gui.cpp \
				direcs-avrsim.cpp \
	interfaceAvr.cpp

    FORMS +=	mainWindow.ui

    DEFINES =	_TTY_POSIX_

    CONFIG -= release

    LIBS += -L/usr/lib \
  -L/usr/local/lib
}
