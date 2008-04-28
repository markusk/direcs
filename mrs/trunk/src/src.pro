# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt.
# -------------------------------------------
# Unterordner relativ zum Projektordner: ./src
# Das Target ist eine Anwendung: ../bin/mrs
MOC_DIR = ../tmp
OBJECTS_DIR = ../tmp
FORMS += mainWindow.ui \
 aboutDialog.ui
HEADERS += head.h \
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
 laserScene.h
SOURCES += head.cpp \
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
 laserScene.cpp
RESOURCES = ../mrs.qrc
DEFINES = _TTY_POSIX_
CONFIG += debug \
    warn_on \
    qt
TEMPLATE = app
unix {
    HEADERS += posix_qextserialport.h \
        QtGLContext.h
    SOURCES += QtGLContext.cpp
}
win32 : HEADERS += win_qextserialport.h
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
TARGET = ../bin/mrs
