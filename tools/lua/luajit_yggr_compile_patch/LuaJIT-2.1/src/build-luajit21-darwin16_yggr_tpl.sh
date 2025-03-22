#!/bin/sh

var_prefix_dir=$1
var_sdk_name=$2
var_sdk_ver_min=$3
var_src_sys=$4
var_src_arch=$5
var_dst_sys=$6
var_dst_arch=$7

if [ "${var_sdk_name}" = "macosx" ]; then
    var_sdk_flag="-mmacosx-version-min=${var_sdk_ver_min}"
elif [ "${var_sdk_name}" = "iphoneos" ]; then
    var_sdk_flag="-miphoneos-version-min=${var_sdk_ver_min}"
else
    var_sdk_flag=""
fi


var_sdk_arch_flag="-arch ${var_dst_arch}"

#src_args
var_sdk_root_src=$(xcrun --sdk macosx --show-sdk-path)
var_cflags_src="-O3 -fPIC -DLUA_USE_DARWIN -arch ${var_src_arch} -isysroot ${var_sdk_root_src} -mmacosx-version-min=12.4"
var_cxxflags_src="-std=c++11 ${var_cflags_src}"
var_ldflags_src="-arch ${var_src_arch}"


#dst_args
var_sdk_root=$(xcrun --sdk ${var_sdk_name} --show-sdk-path)

var_cc=clang
var_cflags="-fPIC -DLUA_USE_DARWIN ${var_sdk_arch_flag} -isysroot ${var_sdk_root} ${var_sdk_flag}"
var_cxxflags="-std=c++11 ${var_cflags}"
var_ldflags="${var_sdk_arch_flag}"

var_prefix_dir_bin="${var_prefix_dir}/bin"
var_prefix_dir_lib="${var_prefix_dir}/lib"
var_prefix_dir_inc="${var_prefix_dir}/include"

if [ "${var_sdk_name}" = "macosx" ]; then
    export MACOSX_DEPLOYMENT_TARGET="${var_sdk_ver_min}"
fi

if [ "${var_sdk_name}" = "macosx" ]; then
    var_build_mode=mixed
else
    var_build_mode=static
fi

#dylib
if [ "${var_build_mode}" = "mixed" ] || [ "${var_build_mode}" = "shared" ]; then
    var_nodotabiver_title="jit501-clang-darwin16"

#   dylib debug
    var_make_cmd="make -j12 -f Makefile_yggr \
        NODOTABIVER=\"${var_nodotabiver_title}-d\" \
        HOST_CC=\"${var_cc}\" \
        HOST_CFLAGS=\"${var_cflags_src}\" \
        HOST_LDFLAGS=\"${var_ldflags_src}\" \
        HOST_SYS=${var_src_sys} \
        TARGET_SYS=${var_dst_sys} \
        BUILDMODE=shared \
        CC=\"${var_cc}\" \
        STATIC_CC=\"${var_cc}\" \
        DYNAMIC_CC=\"${var_cc}\" \
        CFLAGS=\"-g -O0 ${var_cflags}\" \
        LDFLAGS=\"${var_ldflags}\" \
        "

    rm *.o
    rm host/*.o
    eval "${var_make_cmd}"
    mv -f "liblua${var_nodotabiver_title}-d.dylib" "${var_prefix_dir_lib}/liblua${var_nodotabiver_title}-d.dylib"

    var_make_cmd="make -j12 -f Makefile_yggr \
        NODOTABIVER=\"${var_nodotabiver_title}\" \
        HOST_CC=\"${var_cc}\" \
        HOST_CFLAGS=\"${var_cflags_src}\" \
        HOST_LDFLAGS=\"${var_ldflags_src}\" \
        HOST_SYS=${var_src_sys} \
        TARGET_SYS=${var_dst_sys} \
        BUILDMODE=shared \
        CC=\"${var_cc}\" \
        STATIC_CC=\"${var_cc}\" \
        DYNAMIC_CC=\"${var_cc}\" \
        CFLAGS=\"-O3 ${var_cflags}\" \
        LDFLAGS=\"${var_ldflags}\" \
        "

    rm *.o
    rm host/*.o
    eval "${var_make_cmd}"
    mv -f "liblua${var_nodotabiver_title}.dylib" "${var_prefix_dir_lib}/liblua${var_nodotabiver_title}.dylib"

    if [ ! -f "${var_prefix_dir_bin}/lua" ]; then
         cp -f "luajit" "${var_prefix_dir_bin}/lua"
    fi

    if [ "${var_build_mode}" = "shared" ]; then
        cp -f "luajit" "${var_prefix_dir_bin}/luac"
        mv -f "luajit" "${var_prefix_dir_bin}/luajit"
    fi
fi

#static lib
if [ "${var_build_mode}" = "mixed" ] || [ "${var_build_mode}" = "static" ]; then
    var_nodotabiver_title="jit501-clang-darwin16-s"

#   static debug
    var_make_cmd="make -j12 -f Makefile_yggr \
        NODOTABIVER=\"${var_nodotabiver_title}-d\" \
        HOST_CC=\"${var_cc}\" \
        HOST_CFLAGS=\"${var_cflags_src}\" \
        HOST_LDFLAGS=\"${var_ldflags_src}\" \
        HOST_SYS=${var_src_sys} \
        TARGET_SYS=${var_dst_sys} \
        BUILDMODE=static \
        CC=\"${var_cc}\" \
        STATIC_CC=\"${var_cc}\" \
        DYNAMIC_CC=\"${var_cc}\" \
        CFLAGS=\"-g -O0 ${var_cflags}\" \
        LDFLAGS=\"${var_ldflags}\" \
        "

    rm *.o
    rm host/*.o
    eval "${var_make_cmd}"
    mv -f "liblua${var_nodotabiver_title}-d.a" "${var_prefix_dir_lib}/liblua${var_nodotabiver_title}-d.a"

    var_make_cmd="make -j12 -f Makefile_yggr \
        NODOTABIVER=\"${var_nodotabiver_title}\" \
        HOST_CC=\"${var_cc}\" \
        HOST_CFLAGS=\"${var_cflags_src}\" \
        HOST_LDFLAGS=\"${var_ldflags_src}\" \
        HOST_SYS=${var_src_sys} \
        TARGET_SYS=${var_dst_sys} \
        BUILDMODE=static \
        CC=\"${var_cc}\" \
        STATIC_CC=\"${var_cc}\" \
        DYNAMIC_CC=\"${var_cc}\" \
        CFLAGS=\"-O3 ${var_cflags}\" \
        LDFLAGS=\"${var_ldflags}\" \
        "

    rm *.o
    rm host/*.o
    eval "${var_make_cmd}"
    mv -f "liblua${var_nodotabiver_title}.a" "${var_prefix_dir_lib}/liblua${var_nodotabiver_title}.a"

    if [ ! -f "${var_prefix_dir_bin}/lua" ]; then
         cp -f "luajit" "${var_prefix_dir_bin}/lua"
    fi

    cp -f "luajit" "${var_prefix_dir_bin}/luac"
    mv -f "luajit" "${var_prefix_dir_bin}/luajit"
fi

if [ ! -d "${var_prefix_dir_inc}" ]; then
    mkdir -p "${var_prefix_dir_inc}"
    cp -f *.h "${var_prefix_dir_inc}/"
fi