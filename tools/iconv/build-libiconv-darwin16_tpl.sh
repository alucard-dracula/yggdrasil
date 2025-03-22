#!/bin/sh

var_prefix_dir=$1
var_host=$2
var_sdk_name=$3
var_sdk_ver_min=$4

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

var_cc=clang
var_cflags="-O3 -fPIC ${var_sdk_arch_flag} -isysroot ${var_sdk_root} ${var_sdk_flag}"
var_cxxflags="-std=c++11 ${var_cflags}"
var_ldflags="${var_sdk_arch_flag}"

if [ "${var_sdk_name}" = "macosx" ]; then
    ./configure \
        --enable-static=yes \
        --host=${var_host} \
        --prefix=${var_prefix_dir} \
        CC="${var_cc}" \
        CFLAGS="${var_cflags}" \
        LDFLAGS="${var_ldflags}"
else
    ./configure \
        --enable-static=yes \
        --disable-shared \
        --host=${var_host} \
        --prefix=${var_prefix_dir} \
        CC="${var_cc}" \
        CFLAGS="${var_cflags}"
fi

make clean
make -j12
make install