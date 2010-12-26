#!/bin/sh

echo "please provide root passwort for creating symbolic links for serial devices.."

sudo -l ln -s /dev/tty.SLAB_USBtoUART /dev/ttyAtmelBoard

sudo -l ln -s /dev/tty.USA19Hfa141P1.1 /dev/ttyLaserScannerFront
