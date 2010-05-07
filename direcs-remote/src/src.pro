# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src
# Das Target ist eine Anwendung:  ../bin/direcs-remote

FORMS += mainWindow.ui 
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
TARGET = ../bin/direcs-remote
CONFIG += debug \
warn_on \
qt
DEFINES = warn_on \
qt
TEMPLATE = app
QT += network \
      webkit \
 opengl



RESOURCES += ../direcs-remote.qrc
CONFIG -= release

INCLUDEPATH += /opt/local/include/opencv \
			/opt/local/include \
			/opt/local/libexec/qt4-mac/include \
			/usr/include/qwt-qt4 \
			/usr/local/include/opencv


unix|macx {
LIBS += -L/usr/local/lib \
  -lqwt-qt4
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
