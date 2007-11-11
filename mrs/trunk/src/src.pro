# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src
# Das Target ist eine Anwendung:  ../bin/mrs

FORMS += gui.ui 
HEADERS += camThread.h \
           circuit.h \
           gui.h \
           inifile.h \
           motor.h \
           mrs.h \
           sensorThread.h \
           qextserialbase.h \
           qextserialport.h \
           obstacleCheckThread.h \
           interface.h  \
           plotThread.h \
           networkThread.h \
           servo.h \
           laserThread.h \
           speakThread.h \
 sick.h \
 carmenserial.h
SOURCES += camThread.cpp \
           circuit.cpp \
           gui.cpp \
           inifile.cpp \
           motor.cpp \
           mrs.cpp \
           sensorThread.cpp \
           obstacleCheckThread.cpp \
           interface.cpp  \
           plotThread.cpp \
           networkThread.cpp \
           servo.cpp \
           laserThread.cpp \
           speakThread.cpp \
 sick.cpp \
 carmenserial.cpp
RESOURCES = ../mrs.qrc
DEFINES = _TTY_POSIX_
TARGET = ../bin/mrs
CONFIG += debug \
warn_on \
qt
TEMPLATE = app
unix {
    HEADERS += usb_posix.h \
  posix_qextserialport.h \
  QtGLContext.h
    SOURCES += usb_posix.cpp \
  QtGLContext.cpp
}
win32 {
    HEADERS += usb_win.h \
  win_qextserialport.h
    SOURCES += usb_win.cpp
}
QT += network \
opengl
DESTDIR = .

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
/usr/local/qwt/include \
/usr/local/include/opencv
LIBS += -L/usr/local/qwt/lib/ \
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
-lqwt \
-lcv \
-lhighgui
