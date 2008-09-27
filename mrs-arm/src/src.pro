destplatform = $$(DESTPLATFORM)
!isEmpty ( destplatform ) {
	message("+++++++++++++++++++++++++++++++++++++")
	message("Destination platform will be:" $$destplatform)
	CONFIG += destplatform
} else {
	message("-------------------------------------")
	message("Destination platform will be:")
	unix {
		message("Linux")
	}
	win32 {
		message("Windows")
	}
	win32 {
		message("an unhandled platform")
	}
	message("-------------------------------------")
}

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

destplatform {
	message("Processing" $$destplatform "specific scope...")
	# HEADERS -= 
	# SORUCES -= 
	# DEFINES += USE_MY_STUFF QT_DLL
	message("+++++++++++++++++++++++++++++++++++++")
}
