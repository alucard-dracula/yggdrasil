#!/bin/sh

var_outdir=$1
#var_statc_tag=$2

#./runConfigureICU MinGW --prefix="${var_outdir}" --enable-static=yes --enable-tests=no --enable-samples=no

#./runConfigureICU MinGW --prefix="${var_outdir}" --enable-static=yes --enable-tests=no --enable-samples=no --enable-renaming=yes --enable-tracing=yes --enable-rpath=yes --with-data-packaging=static

./runConfigureICU MinGW --prefix="${var_outdir}" --enable-static=yes --enable-tests=no --enable-samples=no --enable-renaming=yes --enable-tracing=yes --enable-rpath=yes

sed -i 's/CURDIR:=$(CURR_FULL_DIR)/CURDIR=\./g' ./data/Makefile
make -j12
make install
