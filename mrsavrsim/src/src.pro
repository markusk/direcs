SOURCES += mrsavrsim.cpp \
           main.cpp
HEADERS += mrsavrsim.h
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt
TARGET = ../bin/mrsavrsim
RESOURCES = application.qrc
INCLUDEPATH += /home/markus/develop/mrs/mrs/trunk/src

