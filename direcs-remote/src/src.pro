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

INCLUDEPATH += /usr/include/qwt-qt4/

LIBS += -L/usr/local/lib \
  -lqwt-qt4

