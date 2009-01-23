destplatform = $$(DESTPLATFORM)
!isEmpty( destplatform )CONFIG += $$destplatform

message("Processing global scope...")

DESTDIR = .

TARGET = ../bin/mrs

MOC_DIR = ../tmp

OBJECTS_DIR = ../tmp

RESOURCES = ../mrs.qrc

CONFIG += debug \
	qt
	
TEMPLATE = app

message("Using special CXXFLAGS instead of warn_on in CONFIG section.")
QMAKE_CXXFLAGS += -W -Wno-parentheses

unix {
	console {
			message("Processing UNIX CONSOLE scope...")
		} else {
			!arm {
				message("Processing UNIX scope...")
				HEADERS +=	aboutDialog.h \
						camThread.h \
						circuit.h \
						direcsSerial.h \h
						gui.h \
						head.h \
						inifile.h \
						interfaceAvr.h \
						joystick.h \
						joystickDialog.h \
						laser.h \
						laserScene.h \
						laserThread.h \
						motor.h \
						mrs.h \
						networkThread.h \
						obstacleCheckThread.h \
						plotThread.h \
						QtGLContext.h \
						sensorThread.h \
						servo.h \
						settingsDialog.h \
						speakThread.h
						
				SOURCES +=	aboutDialog.cpp \
						camThread.cpp \
						circuit.cpp \
						direcsSerial.cpp \
						gui.cpp \
						head.cpp \
						inifile.cpp \
						interfaceAvr.cpp \
						joystick.cpp \
						joystickDialog.cpp \
						laser.cpp \
						laserScene.cpp \
						laserThread.cpp \
						motor.cpp \
						mrs.cpp \
						networkThread.cpp \
						obstacleCheckThread.cpp \
						plotThread.cpp \
						QtGLContext.cpp \
						sensorThread.cpp \
						servo.cpp \
						settingsDialog.cpp \
						speakThread.cpp
						
				FORMS +=	mainWindow.ui \
						aboutDialog.ui \
						joystickDialog.ui \
						settingsDialog.ui
						
				DEFINES =	_TTY_POSIX_
				
				QT +=		network \
						opengl
						
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
				
				LIBS +=		-L/usr/lib \
						-L/usr/local/lib \
						-L/usr/lib/festival \
						-L/usr/lib/speech_tools \
						-lestools \
						-lestbase \
						-leststring \
						-lesd \
						-lFestival \
						-ltermcap \
						-lqwt-qt4 \
						-lcv \
						-lhighgui
			}
	}
}

win32 {
	message("Processing win32 scope...")
	HEADERS +=	aboutDialog.h \
			camThread.h \
			gui.h \
			joystickDialog.h \
			laserScene.h \
			plotThread.h \
			qextserialbase.h \
			qextserialport.h \
			QtGLContext.h \
			settingsDialog.h \
			speakThread.h \
			win_qextserialport.h
			
	SOURCES +=	aboutDialog.cpp \
			camThread.cpp \
			gui.cpp \
			joystickDialog.cpp \
			laserScene.cpp \
			plotThread.cpp \
			QtGLContext.cpp \
			settingsDialog.cpp \
			speakThread.cpp
			
	FORMS +=	mainWindow.ui \
			aboutDialog.ui \
			joystickDialog.ui \
			settingsDialog.ui
			
	DEFINES =	_TTY_WIN_ QWT_DLL QT_DLL
	
	QT +=		network \
			opengl
			
	LIBS +=		-L/usr/lib \
			-L/usr/local/lib \
			-lqwt-qt4
}

arm {
	message("Processing ARM scope...")
	
	CONFIG =	release \
			warn_on \
			qt
			
	DEFINES =	_ARM_
	
	QT -=		opengl \
			gui
	
	QT +=		network
	
	HEADERS +=	circuit.h \
			direcsSerial.h \
			gui_arm.h \
			head.h \
			inifile.h \
			interfaceAvr.h \
			joystick.h \
			laser.h \
			laserThread.h \
			motor.h \
			mrs.h \
			networkThread.h \
			obstacleCheckThread.h \
			sensorThread.h \
			servo.h
			
	SOURCES +=	circuit.cpp \
			direcsSerial.cpp \
			gui_arm.cpp \
			head.cpp \
			inifile.cpp \
			interfaceAvr.cpp \
			joystick.cpp \
			laser.cpp \
			laserThread.cpp \
			motor.cpp \
			mrs.cpp \
			networkThread.cpp \
			obstacleCheckThread.cpp \
			sensorThread.cpp \
			servo.cpp



	LIBS =		-L/usr/lib/ \
			-L/usr/local/lib
			
	FORMS =		mainWindow_arm.ui

	QMAKE_LFLAGS =	-L/usr/local/Trolltech/QtEmbedded-4.4.1-arm/lib -L/home/markus/develop/nslu2/crosstool/gcc-3.4.5-glibc-2.3.6/armv5b-softfloat-linux/armv5b-softfloat-linux/lib
}

console {
	message("Processing CONSOLE scope...")
	
	CONFIG =	release \
			warn_on \
			qt
			
	DEFINES =	_ARM_
	
	QT -=		opengl \
			gui
	
	QT +=		network
	
	HEADERS +=	circuit.h \
			direcsSerial.h \
			gui_arm.h \
			head.h \
			inifile.h \
			interfaceAvr.h \
			joystick.h \
			laser.h \
			laserThread.h \
			motor.h \
			mrs.h \
			networkThread.h \
			obstacleCheckThread.h \
			sensorThread.h \
			servo.h
			
	SOURCES +=	circuit.cpp \
			direcsSerial.cpp \
			gui_arm.cpp \
			head.cpp \
			inifile.cpp \
			interfaceAvr.cpp \
			joystick.cpp \
			laser.cpp \
			laserThread.cpp \
			motor.cpp \
			mrs.cpp \
			networkThread.cpp \
			obstacleCheckThread.cpp \
			sensorThread.cpp \
			servo.cpp



	LIBS =		-L/usr/lib/ \
			-L/usr/local/lib
			
	FORMS =		mainWindow_arm.ui
}
