#!/bin/sh

mplayer -tv driver=v4l2:device=/dev/video0:outfmt=rgb24 tv:///2
