#!/bin/sh

var_outdir=$1
var_cflag=$2
var_cxxflag=$3

export CFLAGS=${var_cflag}
export CXXFLAGS=${var_cxxflag}

./runConfigureICU Linux -prefix="${var_outdir}" --enable-static=yes --enable-tests=no --enable-samples=no --enable-renaming=yes --enable-tracing=yes --enable-rpath=yes

sed -i 's/CURDIR:=$(CURR_FULL_DIR)/CURDIR=\./g' ./data/Makefile
make -j12
make install
