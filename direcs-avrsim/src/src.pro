DESTDIR = ../bin

TARGET = direcs-avrsim

MOC_DIR = ../tmp

OBJECTS_DIR = ../tmp

RESOURCES = ../direcsAvrsim.qrc

CONFIG += debug \
	qt

TEMPLATE = app

HEADERS +=	direcsSerial.h \
	gui.h \
	direcs-avrsim.h \
	interfaceAvr.h \
	simulationThread.h

SOURCES +=	direcsSerial.cpp \
	gui.cpp \
	direcs-avrsim.cpp \
	interfaceAvr.cpp \
	simulationThread.cpp

FORMS +=	mainWindow.ui

CONFIG += debug

INCLUDEPATH += /opt/local/include \
				/opt/local/libexec/qt4-mac/include

LIBS +=		-L/opt/local/lib \
				-L/usr/lib \
				-L/usr/local/lib
