#!/bin/sh

var_prefix_dir=$1
var_sdk_name=$2
var_sdk_ver_min=$3

if [ "${var_sdk_name}" = "macosx" ]; then
    var_sdk_flag="-mmacosx-version-min=${var_sdk_ver_min}"
elif [ "${var_sdk_name}" = "iphoneos" ]; then
    var_sdk_flag="-miphoneos-version-min=${var_sdk_ver_min}"
else
    var_sdk_flag=""
fi


if [ "${var_sdk_name}" = "iphoneos" ]; then
    var_sdk_arch=arm64
    var_sdk_arch_flag="-arch arm64"
else
    var_sdk_arch=x86_64-arm64
    var_sdk_arch_flag="-arch x86_64 -arch arm64"
fi

var_sdk_root=$(xcrun --sdk ${var_sdk_name} --show-sdk-path)

var_BIGFILES=-D_FILE_OFFSET_BITS=64
var_cc=clang
var_cflags="-Wall -Winline -O3 -fPIC ${var_BIGFILES} ${var_sdk_arch_flag} -isysroot ${var_sdk_root} ${var_sdk_flag}"
var_cxxflags="-std=c++11 ${var_cflags}"
var_ldflags="${var_sdk_arch_flag}"

make -j12 clean \
    CC="${var_cc}" \
    CFLAGS="${var_cflags}" \
    LDFLAGS="${var_ldflags}" \
    PREFIX="${var_prefix_dir}"
    
make -j12 \
    CC="${var_cc}" \
    CFLAGS="${var_cflags}" \
    LDFLAGS="${var_ldflags}" \
    PREFIX="${var_prefix_dir}"

make -j12 install \
    CC="${var_cc}" \
    CFLAGS="${var_cflags}" \
    LDFLAGS="${var_ldflags}" \
    PREFIX="${var_prefix_dir}"
