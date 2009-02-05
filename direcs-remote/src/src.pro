# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src
# Das Target ist eine Anwendung:  ../bin/direcs-remote

FORMS += gui.ui 
HEADERS += gui.h \
           direcs-remote.h \
 plotThread.h \
 laserScene.h \
 laserThread.h
SOURCES += gui.cpp \
           direcs-remote.cpp \
 plotThread.cpp \
 laserScene.cpp \
 laserThread.cpp
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

INCLUDEPATH += /usr/include/qwt-qt4/ \
  /home/markus/develop/subversion/applications/direcs-remote/src \
  /usr/include/KDE

LIBS += -L/usr/local/lib \
  -lqwt-qt4

