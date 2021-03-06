#!/bin/sh
# This script has one line for each known working toolchain
# for this architecture.  Uncomment the one you want.
# Generated by generate-demo.pl from buildlogs/all.dats.txt

#
# Markus: DEBIAN (Little Endian) on NSLU2
#
#
# ACHTUNG: VORHER auf dem Host in /usr/bin den gcc SymLink von gcc-4.3 nach gcc-4.1 ändern!
# Crosstool-Build dauert ca. 2 Stunden!
# ACHTUNG: HINTERHER wieder umändern!
# 29.12.2008


set -ex
TARBALLS_DIR=$HOME/download
RESULT_TOP=/home/markus/develop/nslu2/crosstool_debian
export TARBALLS_DIR RESULT_TOP
GCC_LANGUAGES="c,c++"
export GCC_LANGUAGES

# Really, you should do the mkdir before running this,
# and chown /opt/crosstool to yourself so you don't need to run as root.
mkdir -p $RESULT_TOP

#eval `cat arm-xscale.dat gcc-3.2.3-glibc-2.2.5.dat` sh all.sh --notest
#eval `cat arm-xscale.dat gcc-3.2.3-glibc-2.3.2.dat` sh all.sh --notest
#eval `cat arm-xscale.dat gcc-3.2.3-glibc-2.3.2-tls.dat` sh all.sh --notest
#eval `cat arm-xscale.dat gcc-3.3.6-glibc-2.2.5.dat` sh all.sh --notest
#eval `cat arm-xscale.dat gcc-3.3.6-glibc-2.3.2.dat` sh all.sh --notest
#eval `cat arm-xscale.dat gcc-3.3.6-glibc-2.3.2-tls.dat` sh all.sh --notest
#eval `cat arm-xscale.dat gcc-3.4.5-glibc-2.2.5.dat` sh all.sh --notest
#eval `cat arm-xscale.dat gcc-3.4.5-glibc-2.3.2.dat` sh all.sh --notest
#eval `cat arm-xscale.dat gcc-3.4.5-glibc-2.3.2-tls.dat` sh all.sh --notest
#eval `cat arm-xscale.dat gcc-3.4.5-glibc-2.3.5.dat` sh all.sh --notest
#eval `cat arm-xscale.dat gcc-3.4.5-glibc-2.3.5-tls.dat` sh all.sh --notest
#eval `cat arm-xscale.dat gcc-3.4.5-glibc-2.3.6.dat` sh all.sh --notest
#eval `cat arm-xscale.dat gcc-3.4.5-glibc-2.3.6-tls.dat` sh all.sh --notest
#eval `cat arm-xscale.dat gcc-4.0.2-glibc-2.3.2.dat` sh all.sh --notest
#eval `cat arm-xscale.dat gcc-4.0.2-glibc-2.3.2-tls.dat` sh all.sh --notest
#eval `cat arm-xscale.dat gcc-4.0.2-glibc-2.3.5.dat` sh all.sh --notest
#eval `cat arm-xscale.dat gcc-4.0.2-glibc-2.3.5-tls.dat` sh all.sh --notest
#eval `cat arm-xscale.dat gcc-4.0.2-glibc-2.3.6.dat` sh all.sh --notest
#eval `cat arm-xscale.dat gcc-4.0.2-glibc-2.3.6-tls.dat` sh all.sh --notest

# 29.12.2008
# Trying this
eval `cat arm-xscale.dat gcc-4.1.0-glibc-2.3.2.dat` sh all.sh --notest

# ERROR RUNNING MRS:
# 28.12.2008
# ./direcs: relocation error: ./direcs: symbol __subdf3, version GCC_3.0 not defined in file libgcc_s.so.1 with link time reference
# eval `cat arm-xscale.dat gcc-3.4.5-glibc-2.3.6.dat` sh all.sh --notest

echo Done.
