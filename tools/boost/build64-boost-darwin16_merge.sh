#!/bin/sh

var_src_arm64=$1
var_src_x86_64=$2
var_dst_dir=$3

#var_src_root_dir_arm64=stage/ios-arm64
#var_src_root_dir_x86_64=stage/ios-x86_64
#var_dst_root_dir_fat=stage/ios-fat

if [ "${var_src_arm64}" = "" ]; then
    var_src_arm64=./not_exists
fi

if [ "${var_src_x86_64}" = "" ]; then
    var_src_x86_64=./not_exists
fi 

if [ "${var_dst_dir}" = "" ]; then
    var_dst_dir=./not_exists
fi 

rm -fr "${var_dst_dir}"
mkdir -p "${var_dst_dir}"

var_src_lib_list=$(find ${var_src_arm64} -type f -name '*.a')
echo "${var_src_lib_list}"

for var_fpath in ${var_src_lib_list} 
do
    var_fname_arm=${var_fpath##*/}
    var_fname_x86_64=${var_fname_arm//-a64/-x64}
    var_cmd_create="lipo -create ${var_fpath} ${var_src_x86_64}/${var_fname_x86_64} -output ${var_dst_dir}/${var_fname_arm}"
    var_cmd_check="lipo -info ${var_dst_dir}/${var_fname_arm}"
    echo "${var_cmd_create}"
    eval ${var_cmd_create}
    eval ${var_cmd_check}
done

var_src_lib_list=$(find ${var_src_arm64} -type f -name '*.dylib')
echo "${var_src_lib_list}"

for var_fpath in ${var_src_lib_list} 
do
    var_fname_arm=${var_fpath##*/}
    var_fname_x86_64=${var_fname_arm//-a64/-x64}
    var_cmd_create="lipo -create ${var_fpath} ${var_src_x86_64}/${var_fname_x86_64} -output ${var_dst_dir}/${var_fname_arm}"
    var_cmd_check="lipo -info ${var_dst_dir}/${var_fname_arm}"
    echo "${var_cmd_create}"
    eval ${var_cmd_create}
    eval ${var_cmd_check}
done
