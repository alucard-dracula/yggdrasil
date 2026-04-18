#!/bin/bash

export CFLAGS="-O3 -m32"
export LDFLAGS="-s -m32"
#export CPPFLAGS="-O3 -m64"

./configure --prefix=/home/${USER}/third_part/libiconv_117_x32
make
make install
make clean