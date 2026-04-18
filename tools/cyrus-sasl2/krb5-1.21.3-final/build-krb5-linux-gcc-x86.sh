#!/bin/bash

var_dir=$(cd `dirname $0`; pwd)

export CFLAGS=-m32
export LDFLAGS=-m32

var_src_dir=${var_dir}/src
var_build_dir=${var_dir}/src/build-linux-x86
var_stage_dir=${var_build_dir}/stage


var_stage_install=${var_dir}/stage-linux

mkdir -p "${var_stage_install}"

rm -fr "${var_build_dir}"
mkdir -p "${var_stage_dir}"

cd "${var_src_dir}"

rm -f configure
autoreconf --verbose
chmod a+x configure

cd "${var_build_dir}"

../configure --prefix=${var_stage_dir}
make clean
make -j12
make install

#install to last

rm -fr "${var_stage_install}/lib"
rm -fr "${var_stage_install}/bin"
rm -fr "${var_stage_install}/sbin"

#rm -fr "${var_stage_install}/include"
#rm -fr "${var_stage_install}/share"
#rm -fr "${var_stage_install}/var"


cp -fr "${var_stage_dir}/lib" "${var_stage_install}/"
cp -fr "${var_stage_dir}/bin" "${var_stage_install}/"
cp -fr "${var_stage_dir}/sbin" "${var_stage_install}/"

if [ ! -d "${var_stage_install}/include" ]; then
    cp -fr "${var_stage_dir}/include" "${var_stage_install}/"
fi

if [ ! -d "${var_stage_install}/share" ]; then
    cp -fr "${var_stage_dir}/share" "${var_stage_install}/"
fi

if [ ! -d "${var_stage_install}/var" ]; then
    cp -fr "${var_stage_dir}/var" "${var_stage_install}/"
fi

cd "${var_dir}"

