SOURCES += test.cpp \
	main.cpp \
	direcsSerial.cpp \
	inifile.cpp \
	interfaceAvr.cpp \
	circuit.cpp \
	laser.cpp \
	laserSickS300.cpp \
	laserThread.cpp
HEADERS += test.h \
	direcsSerial.h \
	inifile.h \
	interfaceAvr.h \
	circuit.h \
	laser.h \
	laserSickS300.h \
	laserThread.h
TEMPLATE = app
CONFIG += warn_on \
	thread \
	qt \
	debug
TARGET = test
DESTDIR = ../bin
RESOURCES = application.qrc
FORMS +=

QMAKE_CXXFLAGS+= -m32
