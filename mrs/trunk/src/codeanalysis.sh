#!/bin/sh

make clean
find . -regex '.*/*.\(cpp\|h\)' -type f | cccc --outdir=../../cccc -
