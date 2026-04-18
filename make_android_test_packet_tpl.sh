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

cp -f ./make_android_test_link.sh ${var_dst_pak_bin_dir}/
cp -f ./make_android_test_link.sh ${var_dst_pak_bin_debug_dir}/

var_used_android_api=${var_android_api} 
var_boost_ver_tag=-${var_boost_ver_arr[0]}_${var_boost_ver_arr[1]}

if [ "${var_test_abi}" = "arm64-v8a" ]; then
    var_abi_third_part=aarch64-linux-android
    var_boost_platfrom_tag=-a64
elif [ "${var_test_abi}" = "armeabi-v7a" ]; then
    var_abi_third_part=armv7a-linux-androideabi
    var_boost_platfrom_tag=-a32
elif [ "${var_test_abi}" = "riscv64" ]; then
    var_abi_third_part=riscv64-linux-android
    var_boost_platfrom_tag=-r64
    if [ ! ${var_android_api} -ge 35 ]; then
        var_used_android_api=35
    fi
elif [ "${var_test_abi}" = "x86_64" ]; then
    var_abi_third_part=x86_64-linux-android
    var_boost_platfrom_tag=
elif [ "${var_test_abi}" = "x86" ]; then
    var_abi_third_part=i686-linux-android
    var_boost_platfrom_tag=
else
    echo "unkonw api [${var_test_abi}]"
fi

cp -f ./begin_test_env_android_termux.sh ${var_dst_pak_bin_dir}/begin_test_env_android_termux.sh
cp -f ./begin_test_env_android_termux.sh ${var_dst_pak_bin_debug_dir}/begin_test_env_android_termux.sh

#python
cp -fr ${var_ndk_third_part_dir}/python/bin/${var_abi_third_part}/${var_used_android_api}/* ${var_dst_python_bin_dir}/
cp -fr ${var_ndk_third_part_dir}/python/lib/${var_abi_third_part}/${var_used_android_api}/* ${var_dst_python_lib_dir}/

#other
cp -f ${var_ndk_third_part_dir}/libiconv/lib/${var_abi_third_part}/${var_used_android_api}/libiconv.so ${var_dst_pak_bin_debug_dir}/
cp -f ${var_ndk_third_part_dir}/libiconv/lib/${var_abi_third_part}/${var_used_android_api}/libiconv.so ${var_dst_pak_bin_dir}/

cp -f ${var_ndk_third_part_dir}/luabind/lib/${var_abi_third_part}/${var_used_android_api}/libluabind09-clang-d.so ${var_dst_pak_bin_debug_dir}/
cp -f ${var_ndk_third_part_dir}/luabind/lib/${var_abi_third_part}/${var_used_android_api}/libluabind09-clang.so ${var_dst_pak_bin_dir}/

cp -f ${var_ndk_third_part_dir}/lua/lib/${var_abi_third_part}/${var_used_android_api}/liblua${var_lua_ver}-clang-d.so ${var_dst_pak_bin_debug_dir}/
cp -f ${var_ndk_third_part_dir}/lua/lib/${var_abi_third_part}/${var_used_android_api}/liblua${var_lua_ver}-clang.so ${var_dst_pak_bin_dir}/

cp -f ${var_ndk_third_part_dir}/libicu/lib/${var_abi_third_part}/${var_used_android_api}/libicuuc.so.58 ${var_dst_pak_bin_debug_dir}/
cp -f ${var_ndk_third_part_dir}/libicu/lib/${var_abi_third_part}/${var_used_android_api}/libicuuc.so.58 ${var_dst_pak_bin_dir}/

cp -f ${var_ndk_third_part_dir}/libicu/lib/${var_abi_third_part}/${var_used_android_api}/libicui18n.so.58 ${var_dst_pak_bin_debug_dir}/
cp -f ${var_ndk_third_part_dir}/libicu/lib/${var_abi_third_part}/${var_used_android_api}/libicui18n.so.58 ${var_dst_pak_bin_dir}/

cp -f ${var_ndk_third_part_dir}/libicu/lib/${var_abi_third_part}/${var_used_android_api}/libicudata.so.58 ${var_dst_pak_bin_debug_dir}/
cp -f ${var_ndk_third_part_dir}/libicu/lib/${var_abi_third_part}/${var_used_android_api}/libicudata.so.58 ${var_dst_pak_bin_dir}/

#cp -f ${var_ndk_third_part_dir}/libicu/lib/${var_abi_third_part}/${var_used_android_api}/*.so.* ${var_dst_pak_bin_debug_dir}/
#cp -f ${var_ndk_third_part_dir}/libicu/lib/${var_abi_third_part}/${var_used_android_api}/*.so.* ${var_dst_pak_bin_dir}/

#cp -f ${var_ndk_third_part_dir}/libicu/lib/${var_abi_third_part}/${var_used_android_api}/*.so ${var_dst_pak_bin_debug_dir}/
#cp -f ${var_ndk_third_part_dir}/libicu/lib/${var_abi_third_part}/${var_used_android_api}/*.so ${var_dst_pak_bin_dir}/

cp -f ${var_ndk_third_part_dir}/libicu/share/icu/58.2/icudt58l.dat ${var_dst_pak_bin_debug_dir}/
cp -f ${var_ndk_third_part_dir}/libicu/share/icu/58.2/icudt58l.dat ${var_dst_pak_bin_dir}/

cp -f ${var_ndk_third_part_dir}/unixODBC/lib/${var_abi_third_part}/${var_used_android_api}/libodbc.so ${var_dst_pak_bin_debug_dir}/
cp -f ${var_ndk_third_part_dir}/unixODBC/lib/${var_abi_third_part}/${var_used_android_api}/libodbc.so ${var_dst_pak_bin_dir}/

cp -f ${var_ndk_third_part_dir}/openssl/lib/${var_abi_third_part}/${var_used_android_api}/libssl.so ${var_dst_pak_bin_debug_dir}/
cp -f ${var_ndk_third_part_dir}/openssl/lib/${var_abi_third_part}/${var_used_android_api}/libssl.so ${var_dst_pak_bin_dir}/

cp -f ${var_ndk_third_part_dir}/openssl/lib/${var_abi_third_part}/${var_used_android_api}/libcrypto.so ${var_dst_pak_bin_debug_dir}/
cp -f ${var_ndk_third_part_dir}/openssl/lib/${var_abi_third_part}/${var_used_android_api}/libcrypto.so ${var_dst_pak_bin_dir}/

cp -f ./end_test_env_termux.sh ${var_dst_pak_bin_dir}/
cp -f ./end_test_env_termux.sh ${var_dst_pak_bin_debug_dir}/

cp -f ./run_test_android.sh ${var_dst_pak_bin_dir}/
cp -f ./run_test_android.sh ${var_dst_pak_bin_debug_dir}/

mkdir -p ${var_dst_pak_bin_dir}/file_system
mkdir -p ${var_dst_pak_bin_debug_dir}/file_system
cp -fr ./test/file_system/*.txt ${var_dst_pak_bin_dir}/file_system/
cp -fr ./test/file_system/*.txt ${var_dst_pak_bin_debug_dir}/file_system/

mkdir -p ${var_dst_pak_bin_dir}/regex
mkdir -p ${var_dst_pak_bin_debug_dir}/regex
cp -fr ./test/regex/*.txt ${var_dst_pak_bin_dir}/regex/
cp -fr ./test/regex/*.txt ${var_dst_pak_bin_debug_dir}/regex/

mkdir -p ${var_dst_pak_bin_dir}/lua
mkdir -p ${var_dst_pak_bin_debug_dir}/lua
cp -fr ./test/lua/*.lua ${var_dst_pak_bin_dir}/lua/
cp -fr ./test/lua/*.lua ${var_dst_pak_bin_debug_dir}/lua/

mkdir -p ${var_dst_pak_bin_dir}/python
mkdir -p ${var_dst_pak_bin_debug_dir}/python
cp -fr ./test/python/test-clang-release.py ${var_dst_pak_bin_dir}/python/test-clang-release.py
cp -fr ./test/python/test-clang-debug.py ${var_dst_pak_bin_debug_dir}/python/test-clang-debug.py

mkdir -p ${var_dst_pak_bin_dir}/nsql_database_system
mkdir -p ${var_dst_pak_bin_debug_dir}/nsql_database_system
cp -fr ./test/nsql_database_system/*.txt ${var_dst_pak_bin_dir}/nsql_database_system/
cp -fr ./test/nsql_database_system/*.txt ${var_dst_pak_bin_debug_dir}/nsql_database_system/
cp -fr ./test/nsql_database_system/*.mp3 ${var_dst_pak_bin_dir}/nsql_database_system/
cp -fr ./test/nsql_database_system/*.mp3 ${var_dst_pak_bin_debug_dir}/nsql_database_system/
cp -fr ./test/nsql_database_system/*.key ${var_dst_pak_bin_dir}/nsql_database_system/
cp -fr ./test/nsql_database_system/*.key ${var_dst_pak_bin_debug_dir}/nsql_database_system/
cp -fr ./test/nsql_database_system/*.json ${var_dst_pak_bin_dir}/nsql_database_system/
cp -fr ./test/nsql_database_system/*.json ${var_dst_pak_bin_debug_dir}/nsql_database_system/
cp -fr ./test/nsql_database_system/keyfile ${var_dst_pak_bin_dir}/nsql_database_system/
cp -fr ./test/nsql_database_system/keyfile ${var_dst_pak_bin_debug_dir}/nsql_database_system/
cp -fr ./test/nsql_database_system/cert ${var_dst_pak_bin_dir}/nsql_database_system/
cp -fr ./test/nsql_database_system/cert ${var_dst_pak_bin_debug_dir}/nsql_database_system/

