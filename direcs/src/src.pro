TARGET = direcs

DESTDIR = ../bin

MOC_DIR = ../tmp

OBJECTS_DIR = ../tmp

RESOURCES = ../direcs.qrc

CONFIG += warn_on \
	thread \
	qt

TEMPLATE = app

DEFINES += ACTIVELASERVIEW

unix|macx {
	message("Processing UNIX / MAC OS scope...")
	HEADERS +=	aboutDialog.h \
				camThread.h \
				circuit.h \
				compassWidget.h \
				consoleGui.h \
				direcsSerial.h \
				gui.h \
				head.h \
				inifile.h \
				interfaceAvr.h \
				joystick.h \
				joystickDialog.h \
				laser.h \
				laserScene.h \
				laserSickS300.h \
				laserThread.h \
				motor.h \
				direcs.h \
				networkThread.h \
				obstacleCheckThread.h \
				plotThread.h \
				QtGLContext.h \
				sensorThread.h \
				servo.h \
				settingsDialog.h \
				speakThread.h \
				logfile.h

	SOURCES +=	aboutDialog.cpp \
				camThread.cpp \
				circuit.cpp \
				compassWidget.cpp \
				consoleGui.cpp \
				direcsSerial.cpp \
				gui.cpp \
				head.cpp \
				inifile.cpp \
				interfaceAvr.cpp \
				joystick.cpp \
				joystickDialog.cpp \
				laser.cpp \
				laserSickS300.cpp \
				laserScene.cpp \
				laserThread.cpp \
				motor.cpp \
				direcs.cpp \
				networkThread.cpp \
				obstacleCheckThread.cpp \
				plotThread.cpp \
				QtGLContext.cpp \
				sensorThread.cpp \
				servo.cpp \
				settingsDialog.cpp \
				speakThread.cpp \
				logfile.cpp

	FORMS +=	direcs.ui \
				aboutDialog.ui \
				joystickDialog.ui \
				settingsDialog.ui

	QT +=		network \
				opengl

	INCLUDEPATH += /opt/local/include \
				/opt/local/libexec/qt4-mac/include \
				/usr/include/qwt-qt4 \
				/opt/local/include/qwt \
				/usr/local/include \
				/usr/local/include/libfreenect

	LIBS +=		-L/opt/local/lib \
				-L/usr/lib \
				-L/usr/local/lib \
				-lespeak \
				-lqwt-qt4 \
				-lfreenect_sync \
				-lopencv_core \
				-lopencv_highgui \
				-lopencv_imgproc

	QMAKE_CXXFLAGS_DEBUG += -pg
	QMAKE_CXXFLAGS_RELEASE += -pg

	QMAKE_CXXFLAGS+= -msse -msse2 -msse3

#	linux-g++:QMAKE_CXXFLAGS +=  -march=native
#	linux-g++-64:QMAKE_CXXFLAGS +=  -march=native

	message("See http://www.openkinect.org for details how to install libfreenect.")
}


macx {
	message("Processing MAC OS scope...")
	message("Removing espeak support.")
	LIBS -=		-lespeak \
				-lqwt-qt4

	message("Changing qwt lib name. Has to be installed via macports.")
	LIBS +=		-lqwt

	message( Adding joystick stuff... )
	LIBS +=		-framework IOKit \
				-framework Foundation
	HEADERS     += joyreaderMacOS.h
	SOURCES     += joyreaderMacOS.cpp \
				   joyreaderMacOS-objc.m

	ICON = ../images/direcs.icns

	QMAKE_CXXFLAGS+= -arch x86_64
	INCLUDEPATH+=/usr/local/boost/
}


message( Checking if hostname is robot... )
HOSTNAME = $$system(hostname)
message( Hostname is $$HOSTNAME )


contains( HOSTNAME, [rR]obot ) {
#contains( HOSTNAME, iMac ) {
#contains( HOSTNAME, debianmac ) {
	 message( ************************************** )
	 message( *** Configuring for robot build... *** )
	 message( ************************************** )
	 DEFINES += BUILDFORROBOT
	 DEFINES += LASERVIEW

	 message( Removing plot stuff (qwt)... )
	 INCLUDEPATH -= /usr/include/qwt-qt4
	 HEADERS     -= plotThread.h
	 SOURCES     -= plotThread.cpp
	 LIBS        -= -lqwt

	 message( Removing active laser view... )
	 DEFINES -= ACTIVELASERVIEW

	 message( Building RELEASE version (no debugging!)... )
	 CONFIG -= debug
	 CONFIG += release

	 message( Robot configuration finished. )
}


win32 {
	message("****************************")
	message("Sorry guys, no Win* support!")
	message("****************************")
	HEADERS =

	SOURCES =

	FORMS =

	DEFINES =

	LIBS =
}
