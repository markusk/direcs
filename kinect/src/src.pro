QT += opengl
QT += core \
	gui
SOURCES += kinect.cpp \
	main.cpp \
	QKinect.cpp \
    RGBWindow.cpp
HEADERS += kinect.h \
	QKinect.h \
    RGBWindow.h
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
INCLUDEPATH+= /usr/local/include/libfreenect \
/usr/local/include \
/opt/local/include

QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64
macx:INCLUDEPATH+=/usr/local/boost/
linux-g++:QMAKE_CXXFLAGS +=  -march=native
linux-g++-64:QMAKE_CXXFLAGS +=  -march=native

LIBS += -L/usr/local/lib/ -lfreenect

DEPENDPATH+=include
# if we are on a mac define DARWIN
macx:DEFINES += DARWIN
