destplatform = $$(DESTPLATFORM)
!isEmpty ( destplatform ) message("Destination platform will be:" $$destplatform)
!isEmpty ( destplatform ) CONFIG += $$destplatform

SOURCES += mrs-arm.cpp
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

HEADERS += mrs-arm.h \
	qextserialport.h \
 qextserialbase.h

QT -= gui
TARGET = ../bin/mrs-arm

arm {
	message("Processing arm specific scope...")
	# HEADERS -= 
	# SORUCES -= 
	# DEFINES += USE_MY_STUFF QT_DLL
	INCLUDEPATH = /usr/local/Trolltech/QtEmbedded-4.4.1-arm/include
	QMAKE_LFLAGS = -L/usr/local/Trolltech/QtEmbedded-4.4.1-arm/lib -L/home/markus/develop/nslu2/crosstool/gcc-3.4.5-glibc-2.3.6/armv5b-softfloat-linux/armv5b-softfloat-linux/lib  -L/home/markus/develop/qextserialport-arm/build
}
