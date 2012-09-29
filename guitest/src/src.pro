DESTDIR = ../bin

TARGET = guitest

MOC_DIR = ../tmp

OBJECTS_DIR = ../tmp

RESOURCES = ../guitest.qrc

CONFIG += release \
		  qt

TEMPLATE = app

HEADERS +=	gui.h \
			guitest.h \
			laserScene.h
#			QtGLContext.h \

SOURCES +=	gui.cpp \
			guitest.cpp \
			laserScene.cpp
#			QtGLContext.cpp \

FORMS +=	guitest.ui

QT +=		opengl

INCLUDEPATH += /opt/local/include \
			/opt/local/libexec/qt4-mac/include \
			/usr/local/include

LIBS +=		-L/opt/local/lib \
			-L/usr/lib \
			-L/usr/local/lib

ICON = ../images/guitest.icns
