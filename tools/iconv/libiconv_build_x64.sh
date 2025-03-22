#!/bin/sh

export CFLAGS="-O3 -m64"
export LDFLAGS="-s -m64"
#export CPPFLAGS="-O3 -m64"

./configure --prefix=/home/alucard/third_part/libiconv_117_x64
make
make install
make clean

