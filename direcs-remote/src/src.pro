# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src
# Das Target ist eine Anwendung:  ../bin/direcs-remote

FORMS += gui.ui 
HEADERS += gui.h \
           direcs-remote.h
SOURCES += gui.cpp \
           direcs-remote.cpp
TARGET = ../bin/direcs-remote
CONFIG += debug \
warn_on \
qt
DEFINES = warn_on \
qt
TEMPLATE = app
QT += network \
      webkit

INCLUDEPATH += /home/markus/develop/subversion/applications/direcs-remote/src \
		/usr/include/KDE

LIBS += -L/usr/local/lib

RESOURCES += ../direcs-remote.qrc
