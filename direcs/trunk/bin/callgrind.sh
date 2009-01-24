#!/bin/sh

valgrind --tool=callgrind --track-fds=yes --time-stamp=yes ./mrs
