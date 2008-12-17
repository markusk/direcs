SOURCES += mrsavrsim.cpp \
           main.cpp \
 simThread.cpp \
 ..\..\mrs\trunk\src\direcsSerial.cpp \
 ..\..\mrs\trunk\src\interfaceAvr.cpp
HEADERS += mrsavrsim.h \
 simThread.h \
 ..\..\mrs\trunk\src\direcsSerial.h \
 ..\..\mrs\trunk\src\interfaceAvr.h
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt
TARGET = ../bin/mrsavrsim
RESOURCES = application.qrc
INCLUDEPATH += /home/markus/develop/mrs/mrs/trunk/src

DEFINES += _TTY_POSIX_
