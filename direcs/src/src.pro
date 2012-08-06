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
#				compassWidget.h \
				consoleGui.h \
				direcsSerial.h \
				gui.h \
				head.h \
				inifile.h \
				interfaceAvr.h \
				joystick.h \
				joystickDialog.h \
				laserScene.h \
				laserSickS300.h \
				laserThread.h \
				motor.h \
				direcs.h \
				networkThread.h \
				obstacleCheckThread.h \
				partyThread.h \
				plotThread.h \
				QtGLContext.h \
				rgbLed.h \
				sensorThread.h \
				servo.h \
				settingsDialog.h \
				speakThread.h \
				timerThread.h \
				logfile.h

	SOURCES +=	aboutDialog.cpp \
				camThread.cpp \
				circuit.cpp \
#				compassWidget.cpp \
				consoleGui.cpp \
				direcsSerial.cpp \
				gui.cpp \
				head.cpp \
				inifile.cpp \
				interfaceAvr.cpp \
				joystick.cpp \
				joystickDialog.cpp \
				laserSickS300.cpp \
				laserScene.cpp \
				laserThread.cpp \
				motor.cpp \
				direcs.cpp \
				networkThread.cpp \
				obstacleCheckThread.cpp \
				partyThread.cpp \
				plotThread.cpp \
				QtGLContext.cpp \
				rgbLed.cpp \
				sensorThread.cpp \
				servo.cpp \
				settingsDialog.cpp \
				speakThread.cpp \
				timerThread.cpp \
				logfile.cpp

	FORMS +=	direcs.ui \
				aboutDialog.ui \
				joystickDialog.ui \
				settingsDialog.ui \
				direcsSmall.ui

	QT +=		network \
				opengl \
				phonon

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

	message("Changing qwt lib name. Has to be installed via macports or manually (QWT 5.2.1 !!)")
	LIBS +=		-lqwt
	INCLUDEPATH+=/usr/local/qwt-5.2.1/include

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
#contains( HOSTNAME, macbook.fritz.box ) {
#contains( HOSTNAME, iMac ) {
#contains( HOSTNAME, debianmac ) {
	 message( ************************************** )
	 message( *** Configuring for robot build... *** )
	 message( ************************************** )

#	 message( Switching to special robot GUI... )
#	FORMS -=	direcs.ui
#	FORMS +=	direcsSmall.ui

	DEFINES += USEROBOTGUI
#	 DEFINES += BUILDFORROBOT
#	 DEFINES += LASERVIEW

#	 message( Removing plot stuff (qwt)... )
#	 INCLUDEPATH -= /usr/include/qwt-qt4
#	 HEADERS     -= plotThread.h
#	 SOURCES     -= plotThread.cpp
#	 LIBS        -= -lqwt

#	 message( Removing active laser view... )
#	 DEFINES -= ACTIVELASERVIEW

	 message( Building release version (no debugging!)... )
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
