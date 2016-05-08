#!/bin/bash

export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/usr/include/c++/4.7.2/x86_64-slackware-linux/

rm -r build
mkdir build
cd build

qmake ../R2D2.pro -r -spec linux-g++

make
make install
