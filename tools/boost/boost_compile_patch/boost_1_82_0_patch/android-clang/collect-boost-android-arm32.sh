#!/bin/sh

AndroidTargetVersion=21
AndroidTarget=armv7a-linux-androideabi

var_out_root_dir=out_stage/boost_182
var_out_include_dir=${var_out_root_dir}/include
var_out_lib_dir=${var_out_root_dir}/lib/${AndroidTarget}/${AndroidTargetVersion}

mkdir -p ${var_out_include_dir}
mkdir -p ${var_out_lib_dir}

if [ ! -d "${var_out_include_dir}/boost" ]; then
    cp -fr boost ${var_out_include_dir}/
fi 

rm -fr ${var_out_lib_dir}/*
cp -fr stage/lib/* ${var_out_lib_dir}/


