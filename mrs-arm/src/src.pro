SOURCES += main.cpp \
	speak.cpp
TEMPLATE = app
OBJECTS_DIR = obj
MOC_DIR = moc
UI_DIR = uic
CONFIG += warn_on \
	  thread \
          qt
TARGET = ../bin/speakqt
LIBS += -lqextserialport

unix {    DEFINES = _TTY_POSIX_
    HEADERS += posix_qextserialport.h

}win32 : DEFINES = _TTY_WIN_ QWT_DLL QT_DLL


HEADERS += speak.h \
	main.h \
 qextserialport.h \
 qextserialbase.h

SOURCES += main.cpp \
	speak.cpp