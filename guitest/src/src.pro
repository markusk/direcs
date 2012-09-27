TARGET = guitest

DESTDIR = ../bin

MOC_DIR = ../tmp

OBJECTS_DIR = ../tmp

RESOURCES = ../guitest.qrc

CONFIG += warn_on \
	qt

TEMPLATE = app

DEFINES += ACTIVELASERVIEW

unix|macx {
	message("Processing UNIX / MAC OS scope...")
	HEADERS +=	gui.h \
				guitest.h \
				laserScene.h
#				QtGLContext.h \

	SOURCES +=	gui.cpp \
				guitest.cpp \
				laserScene.cpp
#				QtGLContext.cpp \

	FORMS +=	guitest.ui

	QT +=		opengl

	INCLUDEPATH += /opt/local/include \
				/opt/local/libexec/qt4-mac/include \
				/usr/local/include

	LIBS +=		-L/opt/local/lib \
				-L/usr/lib \
				-L/usr/local/lib

	QMAKE_CXXFLAGS_DEBUG += -pg
	QMAKE_CXXFLAGS_RELEASE += -pg

	QMAKE_CXXFLAGS+= -msse -msse2 -msse3
}


macx {
	message("Processing MAC OS scope...")

	ICON = ../images/guitest.icns

	QMAKE_CXXFLAGS+= -arch x86_64
	INCLUDEPATH+=/usr/local/boost/
}
