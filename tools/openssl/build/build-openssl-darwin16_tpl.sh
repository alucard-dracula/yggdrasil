#!/bin/sh

var_prefix_dir=$1
var_openssl_dir=$2
var_ssl_target=$3
var_sdk_name=$4
var_sdk_ver_min=$5


echo "var_prefix_dir ${var_prefix_dir}"
echo "var_openssl_dir ${var_openssl_dir}"
echo "var_ssl_target ${var_ssl_target}"
echo "var_sdk_name ${var_sdk_name}"
echo "var_sdk_ver_min ${var_sdk_ver_min}"


if [ "${var_sdk_name}" = "macosx" ]; then
    var_sdk_flag="-mmacosx-version-min=${var_sdk_ver_min}"
elif [ "${var_sdk_name}" = "iphoneos" ]; then
    var_sdk_flag="-miphoneos-version-min=${var_sdk_ver_min}"
else
    var_sdk_flag=""
fi


#var_cflags="${var_sdk_flag}=${var_sdk_ver_min}"
var_cflags="${var_sdk_flag}"
var_cxxflags="-std=c++11 ${var_cflags}"


export CFLAGS="${var_cflags}"
export CXXFLAGS="${var_cxxflags}"

if [ "${var_sdk_name}" = "macosx" ]; then
    perl Configure \
        ${var_ssl_target} \
        --openssldir=${var_openssl_dir} \
        --prefix=${var_prefix_dir} 
else
    perl Configure \
        no-shared \
        ${var_ssl_target} \
        --openssldir=${var_openssl_dir} \
        --prefix=${var_prefix_dir}
fi

make clean
make -j12
make install
