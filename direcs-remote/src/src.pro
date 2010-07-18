destplatform = $$(DESTPLATFORM)
!isEmpty( destplatform )CONFIG += $$destplatform

DESTDIR = .

TARGET = ../bin/direcs-remote

MOC_DIR = ../tmp

OBJECTS_DIR = ../tmp

RESOURCES = ../direcs-remote.qrc

CONFIG += debug \
			qt

TEMPLATE = app



unix|macx {
	message("Processing UNIX / MAC OS scope...")

	HEADERS += gui.h \
			   direcs-remote.h \
			   plotThread.h \
			   laserScene.h \
			   laserThread.h \
			   network.h

	SOURCES += gui.cpp \
			   direcs-remote.cpp \
			   plotThread.cpp \
			   laserScene.cpp \
			   laserThread.cpp \
			   network.cpp

	FORMS += mainWindow.ui

	QT += network \
		  webkit \
	 opengl

	INCLUDEPATH += /opt/local/include/opencv \
				/opt/local/include \
				/opt/local/libexec/qt4-mac/include \
				/usr/include/qwt-qt4 \
				/opt/local/include/qwt \
				/usr/local/include/opencv

	LIBS +=		-L/opt/local/lib \
				-L/usr/lib \
				-L/usr/local/lib \
				-lqwt-qt4
}


macx {
	message("Processing MAC OS scope...")
	message("Removing espeak support.")
	LIBS -=		-lespeak

	message("Removing OpenCV support.")
	LIBS -=		-lcv \
				-lhighgui

	message("Changing qwt lib name (but has to be installed via macports).")
	LIBS -=		-lqwt-qt4
	LIBS +=		-lqwt


#	message( Adding joystick stuff... )
#	LIBS +=		-framework IOKit \
#				-framework Foundation
#	HEADERS     += joyreaderMacOS.h
#	SOURCES     += joyreaderMacOS.cpp \
#				   joyreaderMacOS-objc.m

	ICON = ../images/direcs-remote.icns
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
