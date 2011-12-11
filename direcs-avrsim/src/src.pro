DESTDIR = ../bin

TARGET = direcs-avrsim

MOC_DIR = ../tmp

OBJECTS_DIR = ../tmp

RESOURCES = ../direcsAvrsim.qrc

CONFIG += release \
	qt

TEMPLATE = app

HEADERS +=	gui.h \
	direcs-avrsim.h \
	interfaceAvr.h \
	simulationThread.h

SOURCES +=	gui.cpp \
	direcs-avrsim.cpp \
	interfaceAvr.cpp \
	simulationThread.cpp

FORMS +=	mainWindow.ui

INCLUDEPATH += /opt/local/include \
				/opt/local/libexec/qt4-mac/include

LIBS +=		-L/opt/local/lib \
				-L/usr/lib \
				-L/usr/local/lib

CONFIG(debug, debug|release):LIBS  += -lqextserialportd
else:LIBS  += -lqextserialport

unix:DEFINES   = _TTY_POSIX_
