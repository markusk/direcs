SUBDIRS += src
TEMPLATE = subdirs 
CONFIG += warn_on \
          qt \
          thread 
 
# INCLUDEPATH = /usr/local/Trolltech/QtEmbedded-4.4.1-arm/include
# QMAKE_LFLAGS = -L/usr/local/Trolltech/QtEmbedded-4.4.1-arm/lib -L/home/markus/develop/nslu2/crosstool/gcc-3.4.5-glibc-2.3.6/armv5b-softfloat-linux/armv5b-softfloat-linux/lib
