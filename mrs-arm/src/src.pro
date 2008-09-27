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
}
