#!/bin/bash

var_usr_cmake_targer=$1
var_darwin_sdk_name=$2
var_darwin_sdk_ver_min=$3
var_darwin_arch=$4
var_usr_cmake_c_standard=$5
var_usr_cmake_cxx_standard=$6
var_usr_cmake_ld_arch_tag=$7
var_clear_other_build_lib=$8
var_clear_other_build_bin=$9

var_usr_cmake_cc=default
var_usr_cmake_cxx=default

#var_usr_cmake_cc=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang
#var_usr_cmake_cxx=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++

var_usr_cmake_compiler_version=clang-darwin16

#var_usr_cmake_c_standard=cstd_ver
#var_usr_cmake_cxx_standard=cppstd_ver

#var_usr_cmake_targer=target

#var_darwin_sdk_name=sdk_name
#var_darwin_sdk_ver_min=min_ver

var_usr_cmake_lib_root_dir=lib

if [ "${var_darwin_sdk_name}" = "iphoneos" ]; then
	var_darwin_arch="arm64"
else
	if [ "${var_darwin_arch}" = "" ] || [ "${var_darwin_arch}" = "all" ]; then
		var_darwin_arch="x86_64 -arch arm64"
	fi

fi

if [[ "${var_darwin_arch}" =~ "arm64" ]]; then
	var_darwin_arch_tag=a
else
	var_darwin_arch_tag=x
fi

# 64bit debug release
#var_usr_cmake_ld_arch_tag=x
var_usr_cmake_arch_bits=64
var_usr_cmake_build_type=Debug

if [[ "${var_darwin_arch}" =~ "-arch" ]] || [ "${var_darwin_sdk_name}" = "iphoneos" ]; then
	var_usr_cmake_mid_dir=cmake_build_cache-${var_darwin_sdk_name}-${var_usr_cmake_targer}-${var_usr_cmake_build_type}
else
	var_usr_cmake_mid_dir=cmake_build_cache-${var_darwin_sdk_name}-${var_darwin_arch}-${var_usr_cmake_targer}-${var_usr_cmake_build_type}
fi 

var_usr_cmake_bin_root_dir=${var_usr_cmake_build_type}-${var_darwin_arch_tag}${var_usr_cmake_arch_bits}

if [ "${var_clear_other_build_lib}" = "yes" ] && [ "${var_usr_cmake_targer}" = "Ninja" ]; then
	if [ -d "../../${var_usr_cmake_lib_root_dir}" ]; then
		rm -fr "../../${var_usr_cmake_lib_root_dir}"
		var_clear_other_build_bin="yes"
	fi
fi

if [ "${var_clear_other_build_bin}" = "yes" ] && [ "${var_usr_cmake_targer}" = "Ninja" ]; then
	if [ -d "../../${var_usr_cmake_bin_root_dir}" ]; then
		rm -fr "../../${var_usr_cmake_bin_root_dir}"
	fi
fi

sh cmake-build-tpl.sh \
	${var_usr_cmake_cc} \
	${var_usr_cmake_cxx} \
	${var_usr_cmake_compiler_version} \
	${var_usr_cmake_c_standard} \
	${var_usr_cmake_cxx_standard} \
	${var_usr_cmake_ld_arch_tag} \
	${var_usr_cmake_arch_bits} \
	${var_usr_cmake_build_type} \
	${var_usr_cmake_mid_dir} \
	${var_usr_cmake_targer} \
	${var_darwin_sdk_name} \
	${var_darwin_sdk_ver_min} \
	"${var_darwin_arch}"

var_usr_cmake_build_type=Release
if [[ "${var_darwin_arch}" =~ "-arch" ]] || [ "${var_darwin_sdk_name}" = "iphoneos" ]; then
	var_usr_cmake_mid_dir=cmake_build_cache-${var_darwin_sdk_name}-${var_usr_cmake_targer}-${var_usr_cmake_build_type}
else
	var_usr_cmake_mid_dir=cmake_build_cache-${var_darwin_sdk_name}-${var_darwin_arch}-${var_usr_cmake_targer}-${var_usr_cmake_build_type}
fi 

var_usr_cmake_bin_root_dir=${var_usr_cmake_build_type}-${var_darwin_arch_tag}${var_usr_cmake_arch_bits}

if [ "${var_clear_other_build_bin}" = "yes" ] && [ "${var_usr_cmake_targer}" = "Ninja" ]; then
	if [ -d "../../${var_usr_cmake_bin_root_dir}" ]; then
		rm -fr "../../${var_usr_cmake_bin_root_dir}"
	fi
fi

sh cmake-build-tpl.sh \
	${var_usr_cmake_cc} \
	${var_usr_cmake_cxx} \
	${var_usr_cmake_compiler_version} \
	${var_usr_cmake_c_standard} \
	${var_usr_cmake_cxx_standard} \
	${var_usr_cmake_ld_arch_tag} \
	${var_usr_cmake_arch_bits} \
	${var_usr_cmake_build_type} \
	${var_usr_cmake_mid_dir} \
	${var_usr_cmake_targer} \
	${var_darwin_sdk_name} \
	${var_darwin_sdk_ver_min} \
	"${var_darwin_arch}"

# xcode10 and later not support 32bit abi
