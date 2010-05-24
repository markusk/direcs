destplatform = $$(DESTPLATFORM)
!isEmpty( destplatform )CONFIG += $$destplatform

DESTDIR = .

TARGET = ../bin/direcs

MOC_DIR = ../tmp

OBJECTS_DIR = ../tmp

RESOURCES = ../direcs.qrc

CONFIG += debug \
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

    FORMS +=	mainWindow.ui \
				aboutDialog.ui \
				joystickDialog.ui \
				settingsDialog.ui

	QT +=		network \
				opengl

	INCLUDEPATH += /opt/local/include/opencv \
				/opt/local/include \
				/opt/local/libexec/qt4-mac/include \
				/usr/include/qwt-qt4 \
				/usr/local/include/opencv

    CONFIG -= release

	LIBS +=		-L/opt/local/lib \
				-L/opt/local/lib \
				-L/usr/lib \
				-L/usr/local/lib \
				-lespeak \
				-lqwt-qt4 \
				-lcv \
				-lhighgui

    OBJECTS_DIR = ../tmp

    MOC_DIR = ../tmp
    QMAKE_CXXFLAGS_DEBUG += -pg
    QMAKE_CXXFLAGS_RELEASE += -pg

}


macx {
	message("Processing MAC OS scope...")
	message("Removing espeak support.")
	LIBS -=		-lespeak \
				-lqwt-qt4

	message("Removing OpenCV support.")
	LIBS -=		-lcv \
				-lhighgui

	message("Changing qwt lib name. Has to be installed via macports.")
	LIBS +=		-lqwt

	ICON = ../images/direcs.icns
}


message( Checking if hostname is robot... )
HOSTNAME = $$system(hostname)
message( Hostname is $$HOSTNAME )


contains( HOSTNAME, [rR]obot ) {
#contains( HOSTNAME, imac ) {
	 message( *** Configuring for robot build... *** )
	 DEFINES += BUILDFORROBOT
	 DEFINES += LASERVIEW

	 message( Removing plot stuff (qwt)... )
	 INCLUDEPATH -= /usr/include/qwt-qt4
	 HEADERS     -= plotThread.h
	 SOURCES     -= plotThread.cpp
	 LIBS        -= -lqwt

	 message( Removing active laser view... )
	 DEFINES -= ACTIVELASERVIEW

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

arm {
    message("Processing ARM scope...")
    QMAKE_LFLAGS =	-L/usr/local/Trolltech/QtEmbedded-4.4.1-arm/lib -L/home/markus/develop/nslu2/crosstool/gcc-3.4.5-glibc-2.3.6/armv5b-softfloat-linux/armv5b-softfloat-linux/lib
}
