SOURCES += test.cpp \
	main.cpp \
	direcsSerialQext.cpp \
	inifile.cpp \
	interfaceAvr.cpp \
	circuit.cpp \
	sensorThread.cpp \
	timerThread.cpp
#	laser.cpp \
#	laserSickS300.cpp \
#	laserThread.cpp
HEADERS += test.h \
	direcsSerialQext.h \
	inifile.h \
	interfaceAvr.h \
	circuit.h \
	sensorThread.h \
	timerThread.h
#	laser.h \
#	laserSickS300.h \
#	laserThread.h
TEMPLATE = app
CONFIG += warn_on \
	thread \
	qt \
	release \
	extserialport
TARGET = test
DESTDIR = ../bin
RESOURCES = application.qrc
FORMS +=

# QMAKE_CXXFLAGS+= -m32

MOC_DIR = ../tmp

OBJECTS_DIR = ../tmp

