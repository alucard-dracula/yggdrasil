#!/bin/bash

var_src_pak_dir=$1
var_dst_pak_dir=$2
var_test_abi=$3
var_android_api=$4
var_lua_ver=$5
var_python_ver=$6
var_boost_ver=$7

var_boost_ver_arr=($(echo "$var_boost_ver" | tr '.' ' '))
var_python_ver_float=${var_python_ver:0:1}.${var_python_ver:0-2} 
var_python_ver_main=${var_python_ver:0:1}

var_ndk_third_part_dir=/d/android_devel/ndk_third_part_local

var_src_pak_bin_dir=${var_src_pak_dir}/bin/${var_test_abi}
var_src_pak_bin_debug_dir=${var_src_pak_dir}/bin-debug/${var_test_abi}

var_dst_pak_bin_dir=${var_dst_pak_dir}/bin
var_dst_pak_bin_debug_dir=${var_dst_pak_dir}/bin-debug

var_dst_python_root_dir=${var_dst_pak_dir}/python
var_dst_python_bin_dir=${var_dst_python_root_dir}/bin
var_dst_python_lib_dir=${var_dst_python_root_dir}/lib

rm -fr ${var_dst_pak_dir}

mkdir -p ${var_dst_pak_bin_dir}
mkdir -p ${var_dst_pak_bin_debug_dir}

mkdir -p ${var_dst_python_bin_dir}
mkdir -p ${var_dst_python_lib_dir}

cp -fr ${var_src_pak_bin_dir}/* ${var_dst_pak_bin_dir}/
cp -fr ${var_src_pak_bin_debug_dir}/* ${var_dst_pak_bin_debug_dir}/

cp -f ./lua_hello_dll/test_lua_hello_dll.lua ${var_dst_pak_bin_dir}/
cp -f ./lua_active_svr_test/cal_num_test.lua ${var_dst_pak_bin_dir}/
cp -f ./lua_clt_test/create_cdt.lua ${var_dst_pak_bin_dir}/

cp -f ./lua_hello_dll/test_lua_hello_dll.lua ${var_dst_pak_bin_debug_dir}/
cp -f ./lua_active_svr_test/cal_num_test.lua ${var_dst_pak_bin_debug_dir}/
cp -f ./lua_clt_test/create_cdt.lua ${var_dst_pak_bin_debug_dir}/

cp -f ./make_android_test_svr_clt_link.sh ${var_dst_pak_bin_dir}/
cp -f ./make_android_test_svr_clt_link.sh ${var_dst_pak_bin_debug_dir}/

var_used_android_api=${var_android_api} 
var_boost_ver_tag=-${var_boost_ver_arr[0]}_${var_boost_ver_arr[1]}

if [ "${var_test_abi}" = "arm64-v8a" ]; then
    var_abi_third_part=aarch64-linux-android
elif [ "${var_test_abi}" = "armeabi-v7a" ]; then
    var_abi_third_part=armv7a-linux-androideabi
elif [ "${var_test_abi}" = "riscv64" ]; then
    var_abi_third_part=riscv64-linux-android
    if [ ! ${var_android_api} -ge 35 ]; then
        var_used_android_api=35
    fi
elif [ "${var_test_abi}" = "x86_64" ]; then
    var_abi_third_part=x86_64-linux-android
elif [ "${var_test_abi}" = "x86" ]; then
    var_abi_third_part=i686-linux-android
else
    echo "unkonw api [${var_test_abi}]"
fi

cp -f ./begin_test_env_android_termux.sh ${var_dst_pak_bin_dir}/begin_test_env_android_termux.sh
cp -f ./begin_test_env_android_termux.sh ${var_dst_pak_bin_debug_dir}/begin_test_env_android_termux.sh

#python
cp -fr ${var_ndk_third_part_dir}/python/bin/${var_abi_third_part}/${var_used_android_api}/* ${var_dst_python_bin_dir}/
cp -fr ${var_ndk_third_part_dir}/python/lib/${var_abi_third_part}/${var_used_android_api}/* ${var_dst_python_lib_dir}/

cp -f ${var_ndk_third_part_dir}/libiconv/lib/${var_abi_third_part}/${var_used_android_api}/libiconv.so ${var_dst_pak_bin_debug_dir}/
cp -f ${var_ndk_third_part_dir}/libiconv/lib/${var_abi_third_part}/${var_used_android_api}/libiconv.so ${var_dst_pak_bin_dir}/

cp -f ${var_ndk_third_part_dir}/luabind/lib/${var_abi_third_part}/${var_used_android_api}/libluabind09-clang-d.so ${var_dst_pak_bin_debug_dir}/
cp -f ${var_ndk_third_part_dir}/luabind/lib/${var_abi_third_part}/${var_used_android_api}/libluabind09-clang.so ${var_dst_pak_bin_dir}/

cp -f ${var_ndk_third_part_dir}/lua/lib/${var_abi_third_part}/${var_used_android_api}/liblua${var_lua_ver}-clang-d.so ${var_dst_pak_bin_debug_dir}/
cp -f ${var_ndk_third_part_dir}/lua/lib/${var_abi_third_part}/${var_used_android_api}/liblua${var_lua_ver}-clang.so ${var_dst_pak_bin_dir}/

#cp -f ${var_ndk_third_part_dir}/lua/bin-debug/${var_test_abi}/lua ${var_dst_pak_bin_debug_dir}/
#cp -f ${var_ndk_third_part_dir}/lua/bin/${var_test_abi}/lua ${var_dst_pak_bin_dir}/

#cp -f ${var_ndk_third_part_dir}/lua/bin-debug/${var_test_abi}/luac ${var_dst_pak_bin_debug_dir}/
#cp -f ${var_ndk_third_part_dir}/lua/bin/${var_test_abi}/luac ${var_dst_pak_bin_dir}/

cp -f ./end_test_env_termux.sh ${var_dst_pak_bin_dir}/
cp -f ./end_test_env_termux.sh ${var_dst_pak_bin_debug_dir}/

