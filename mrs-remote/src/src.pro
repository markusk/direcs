# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src
# Das Target ist eine Anwendung:  ../bin/mrs-remote

FORMS += gui.ui 
HEADERS += gui.h \
           mrs-remote.h
SOURCES += gui.cpp \
           mrs-remote.cpp
TARGET = ../bin/mrs-remote
CONFIG += debug \
warn_on \
qt
DEFINES = warn_on \
qt
TEMPLATE = app
QT += network \
      webkit

INCLUDEPATH += /home/markus/develop/subversion/applications/mrs-remote/src \
		/usr/include/KDE

LIBS += -L/usr/local/lib

RESOURCES += ../mrs-remote.qrc
