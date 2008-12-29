destplatform = $$(DESTPLATFORM)
!isEmpty( destplatform ) message("Destination platform will be:" $$destplatform)
!isEmpty( destplatform )CONFIG += $$destplatform

HEADERS +=	head.h \
		camThread.h \
		circuit.h \
		gui.h \
		inifile.h \
		motor.h \
		mrs.h \
		sensorThread.h \
		qextserialbase.h \
		qextserialport.h \
		obstacleCheckThread.h \
		interfaceAvr.h \
		plotThread.h \
		networkThread.h \
		laserThread.h \
		speakThread.h \
		direcsSerial.h \
		laser.h \
		joystick.h \
		servo.h \
		aboutDialog.h \
		laserScene.h \
		joystickDialog.h \
		settingsDialog.h
SOURCES +=	head.cpp \
		camThread.cpp \
		circuit.cpp \
		gui.cpp \
		inifile.cpp \
		motor.cpp \
		mrs.cpp \
		sensorThread.cpp \
		obstacleCheckThread.cpp \
		interfaceAvr.cpp \
		plotThread.cpp \
		networkThread.cpp \
		laserThread.cpp \
		speakThread.cpp \
		direcsSerial.cpp \
		laser.cpp \
		joystick.cpp \
		servo.cpp \
		aboutDialog.cpp \
		laserScene.cpp \
		joystickDialog.cpp \
		settingsDialog.cpp
INCLUDEPATH +=	/usr/include/atk-1.0/ \
		/usr/lib/gtk-2.0/include/ \
		/usr/lib/glib-2.0/include/ \
		/usr/lib/glib/include/ \
		/usr/include/glib-2.0/ \
		/usr/include/pango-1.0/ \
		/usr/include/cairo/ \
		/usr/include/gtk-2.0/ \
		/usr/include/speech_tools \
		/usr/include/festival \
		/usr/include/qwt-qt4 \
		/usr/local/include/opencv

message("Processing default stuff...")
DESTDIR = .
TARGET = ../bin/mrs
MOC_DIR = ../tmp
OBJECTS_DIR = ../tmp
FORMS += aboutDialog.ui \
	joystickDialog.ui \
	settingsDialog.ui
RESOURCES = ../mrs.qrc
CONFIG += debug \
    warn_on \
    qt
TEMPLATE = app
CONFIG -= release

LIBS += -L/usr/lib/festival \
  -L/usr/lib/speech_tools \
  -lestools \
  -lestbase \
  -leststring \
  -lesd \
  -lFestival \
  -lqextserialport \
  -lqwt-qt4 \
  -lcv \
  -lhighgui


unix {
    HEADERS +=	posix_qextserialport.h \
			QtGLContext.h
    SOURCES +=	QtGLContext.cpp
    FORMS +=	mainWindow.ui
    DEFINES =	_TTY_POSIX_
    QT +=		network \
			opengl
    INCLUDEPATH += /usr/include/atk-1.0/ \
  /usr/lib/gtk-2.0/include/ \
  /usr/lib/glib-2.0/include/ \
  /usr/lib/glib/include/ \
  /usr/include/glib-2.0/ \
  /usr/include/pango-1.0/ \
  /usr/include/cairo/ \
  /usr/include/gtk-2.0/ \
  /usr/include/speech_tools \
  /usr/include/festival \
  /usr/include/qwt-qt4 \
  /usr/local/include/opencv

    LIBS += -L/usr/lib \
  -L/usr/local/lib \
  -L/usr/lib/festival \
  -L/usr/lib/speech_tools \
  -lestools \
  -lestbase \
  -leststring \
  -lesd \
  -lFestival \
  -ltermcap \
  -lqextserialport \
  -lqwt-qt4 \
  -lcv \
  -lhighgui

}

win32 {
    HEADERS +=	win_qextserialport.h
    FORMS +=	mainWindow.ui
    DEFINES =	_TTY_WIN_ QWT_DLL QT_DLL
    QT +=		network \
			opengl
}

arm {
    message("Processing additional ARM specific scope...")
    CONFIG = release \
    warn_on \
    qt
    DEFINES = _ARM_
    QT -=	opengl \
    		gui
    HEADERS -=	camThread.h \
			speakThread.h \
			plotThread.h \
			QtGLContext.h \
			laserScene.h \
			aboutDialog.h \
			joystickDialog.h \
			settingsDialog.h \
			gui.h
    HEADERS +=	gui_arm.h
    SOURCES -=	camThread.cpp \
			speakThread.cpp \
			plotThread.cpp \
			QtGLContext.cpp \
			laserScene.cpp \
			aboutDialog.cpp \
			joystickDialog.cpp \
			settingsDialog.cpp \
			gui.cpp
    SOURCES +=	gui_arm.cpp
    INCLUDEPATH -=	/usr/include/atk-1.0/ \
			/usr/lib/gtk-2.0/include/ \
			/usr/include/pango-1.0/ \
			/usr/include/cairo/ \
			/usr/include/gtk-2.0/ \
			/usr/include/speech_tools \
			/usr/include/festival \
			/usr/local/include/opencv
    LIBS =		-L/usr/lib/ \
			-L/usr/local/lib
    FORMS =	mainWindow_arm.ui
    QMAKE_LFLAGS = -L/usr/local/Trolltech/QtEmbedded-4.4.1-arm/lib -L/home/markus/develop/nslu2/crosstool/gcc-3.4.5-glibc-2.3.6/armv5b-softfloat-linux/armv5b-softfloat-linux/lib
}

QMAKE_CXXFLAGS = -Wno-parentheses
