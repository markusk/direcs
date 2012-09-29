DESTDIR = ../bin

TARGET = guitest

MOC_DIR = ../tmp

OBJECTS_DIR = ../tmp

RESOURCES = ../guitest.qrc

CONFIG += release \
	qt

TEMPLATE = app

HEADERS +=	gui.h \
	guitest.h

SOURCES +=	gui.cpp \
	guitest.cpp

FORMS +=	mainWindow.ui

INCLUDEPATH += /opt/local/include \
				/opt/local/libexec/qt4-mac/include

LIBS +=		-L/opt/local/lib \
				-L/usr/lib \
				-L/usr/local/lib
