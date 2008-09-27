destplatform = $$(DESTPLATFORM)
!isEmpty ( destplatform ) message("Destination platform will be:" $$destplatform)
!isEmpty ( destplatform ) CONFIG += $$destplatform

unix {
	HEADERS +=	posix_qextserialport.h \
			QtGLContext.h
			head.h \
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
			sick.h \
			carmenserial.h \
			laser_main.h \
			param_interface.h \
			joystick.h \
			laser_messages.h \
			servo.h \
			heartbeat.h \
			aboutDialog.h \
			laserScene.h \
			joystickDialog.h \
			settingsDialog.h
	SOURCES +=	QtGLContext.cpp \
			head.cpp \
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
			sick.cpp \
			carmenserial.cpp \
			laser_main.cpp \
			joystick.cpp \
			servo.cpp \
			heartbeat.cpp \
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
	LIBS += 	-L/usr/lib/ \
			-L/usr/local/lib \
			-L/usr/lib/festival/ \
			-L/usr/lib/speech_tools/ \
			-lFestival \
			-lestools \
			-lestbase \
			-leststring \
			-ltermcap \
			-lesd \
			-lqextserialport \
			-lqwt-qt4 \
			-lcv \
			-lhighgui
	DEFINES =	_TTY_POSIX_
}

win32 {
	HEADERS +=	win_qextserialport.h \
			head.h \
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
			sick.h \
			carmenserial.h \
			laser_main.h \
			param_interface.h \
			joystick.h \
			laser_messages.h \
			servo.h \
			heartbeat.h \
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
			sick.cpp \
			carmenserial.cpp \
			laser_main.cpp \
			joystick.cpp \
			servo.cpp \
			heartbeat.cpp \
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
	LIBS += 	-L/usr/lib/ \
			-L/usr/local/lib \
			-L/usr/lib/festival/ \
			-L/usr/lib/speech_tools/ \
			-lFestival \
			-lestools \
			-lestbase \
			-leststring \
			-ltermcap \
			-lesd \
			-lqextserialport \
			-lqwt-qt4 \
			-lcv \
			-lhighgui
	DEFINES =	_TTY_WIN_ QWT_DLL QT_DLL
}

arm {
	message("Processing arm specific scope...")
	HEADERS -=	camThread.h \
			speakThread.h
	SOURCES -=	camThread.cpp \
			speakThread.cpp
	INCLUDEPATH +=	/usr/include/qwt-qt4
	LIBS += 	-L/usr/lib/ \
			-L/usr/local/lib \
			-ltermcap \
			-lesd \
			-lqextserialport \
			-lqwt-qt4 \
	DEFINES += _ARM_
}

QT += network \
    opengl
DESTDIR = .
TARGET = ../bin/mrs
MOC_DIR = ../tmp
OBJECTS_DIR = ../tmp
FORMS += mainWindow.ui \
	aboutDialog.ui \
	joystickDialog.ui \
	settingsDialog.ui
RESOURCES = ../mrs.qrc
CONFIG += debug \
    warn_on \
    qt
TEMPLATE = app
