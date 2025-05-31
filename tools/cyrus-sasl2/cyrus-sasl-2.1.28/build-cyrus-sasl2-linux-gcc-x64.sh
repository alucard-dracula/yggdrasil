#!/bin/bash

var_dir=$(cd `dirname $0`; pwd)

export CFLAGS="-m64 -DPROTOTYPES=1 -DHAVE_TIME_H=1"
export LDFLAGS="-m64"

var_build_dir=${var_dir}/build-linux-x64
var_stage_dir=${var_build_dir}/stage

var_stage_install=${var_dir}/stage-linux

mkdir -p "${var_stage_install}"

rm -fr "${var_build_dir}"
mkdir -p "${var_stage_dir}"

cd "${var_build_dir}"

../configure --prefix=${var_stage_dir}
make clean
make -j12
make install

#install to last

rm -fr "${var_stage_install}/lib64"
rm -fr "${var_stage_install}/sbin64"

rm -fr "${var_stage_install}/include"
rm -fr "${var_stage_install}/share"


cp -fr "${var_stage_dir}/lib" "${var_stage_install}/lib64"
cp -fr "${var_stage_dir}/sbin" "${var_stage_install}/sbin64"

cp -fr "${var_stage_dir}/include" "${var_stage_install}/"
cp -fr "${var_stage_dir}/share" "${var_stage_install}/"

cd "${var_dir}/"