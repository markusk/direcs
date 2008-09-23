SOURCES += speak.cpp
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt
LIBS += -lqextserialport

unix {    DEFINES = _TTY_POSIX_
    HEADERS += posix_qextserialport.h
}

win32 {    DEFINES = _TTY_WIN_ QWT_DLL QT_DLL

}

HEADERS += speak.h \
	qextserialport.h \
 qextserialbase.h

QT -= gui
TARGET = ../bin/speak
