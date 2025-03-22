#!/bin/bash

var_usr_cmake_cc=$1
var_usr_cmake_cxx=$2
var_usr_cmake_compiler_version=$3

var_usr_cmake_c_standard=$4
var_usr_cmake_cxx_standard=$5

var_usr_cmake_ld_arch_tag=$6
var_usr_cmake_arch_bits=$7
var_usr_cmake_build_type=$8
var_usr_cmake_mid_dir=$9

shift
var_usr_cmake_target=$9

shift
var_darwin_sdk_name=$9
shift
var_darwin_sdk_ver_min=$9
shift
var_darwin_arch=$9

if [ "${var_usr_cmake_cc}" = "default" ]; then
	var_usr_cmake_cc=""
fi

if [ "${var_usr_cmake_cxx}" = "default" ]; then
	var_usr_cmake_cxx=""
fi

if [ ! "${var_usr_cmake_compiler_version}" = "" ]; then
	var_usr_cmake_compiler_version_tag=-${var_usr_cmake_compiler_version}
else
	var_usr_cmake_compiler_version_tag=""
fi

var_usrdef_cmake_osx_sysroot=$(xcrun --sdk ${var_darwin_sdk_name} --show-sdk-path)

if [ "${var_darwin_sdk_name}" = "iphoneos" ]; then
	var_darwin_arch="arm64"
	var_usrdef_cflag="-miphoneos-version-min=${var_darwin_sdk_ver_min}"
else
	if [ "${var_darwin_arch}" = "" ] || [ "${var_darwin_arch}" = "all" ]; then
		var_darwin_arch="x86_64 -arch arm64"
	fi

	if [ "${var_darwin_sdk_name}" = "macosx" ]; then
		var_usrdef_cflag="-mmacosx-version-min=${var_darwin_sdk_ver_min}"
	else
		var_usrdef_cflag=""
	fi
fi

var_usrdef_cmake_osx_architectures="${var_darwin_arch}"

if [[ "${tvar_darwin_arch}" =~ "arm64" ]]; then 
	var_usr_cmake_arch_tag=a
else
	var_usr_cmake_arch_tag=x
fi

var_usr_cmake_arch_bits=64
var_usr_cmake_arch_bits_tag=-${var_usr_cmake_arch_tag}${var_usr_cmake_arch_bits}

if [ ! "${var_usr_cmake_build_type}" = "Debug" ]; then
	var_usr_cmake_build_type="Release"
fi

var_usr_cmake_bin_root_dir=${var_usr_cmake_build_type}${var_usr_cmake_arch_bits_tag}
var_usr_cmake_bin_outdir=${var_usr_cmake_bin_root_dir}/${var_usr_cmake_compiler_version}

# macos clang not support 32 bit

var_usr_cmake_lib_outdir=lib

if [ -d "${var_usr_cmake_mid_dir}" ]; then
	rm -fr "${var_usr_cmake_mid_dir}"
fi

mkdir -p "${var_usr_cmake_mid_dir}"

cd ${var_usr_cmake_mid_dir}

cmake \
	-G ${var_usr_cmake_target} \
	.. \
	-DUSRDEF_CMAKE_C_COMPILER=${var_usr_cmake_cc} \
	-DUSRDEF_CMAKE_CXX_COMPILER=${var_usr_cmake_cxx} \
	-DUSRDEF_CMAKE_COMPILER_VERSION=${var_usr_cmake_compiler_version} \
	-DUSRDEF_CMAKE_BIN_OUTDIR=${var_usr_cmake_bin_outdir} \
	-DUSRDEF_CMAKE_LIB_OUTDIR=${var_usr_cmake_lib_outdir} \
	-DUSRDEF_CMAKE_C_STANDARD=${var_usr_cmake_c_standard} \
	-DUSRDEF_CMAKE_CXX_STANDARD=${var_usr_cmake_cxx_standard} \
	-DUSRDEF_CMAKE_BUILD_TYPE=${var_usr_cmake_build_type} \
	-DUSRDEF_CMAKE_BUILD_LD_ARCH_TAG=${var_usr_cmake_ld_arch_tag} \
	-DUSRDEF_CMAKE_BUILD_ARCH_BITS=${var_usr_cmake_arch_bits} \
	-DUSRDEF_CMAKE_OSX_SDK_NAME="${var_darwin_sdk_name}" \
	-DUSRDEF_CMAKE_OSX_SYSROOT="${var_usrdef_cmake_osx_sysroot}" \
	-DUSRDEF_CMAKE_OSX_ARCHITECTURES="${var_usrdef_cmake_osx_architectures}" \
	-DUSRDEF_CMAKE_CFLAGS="${var_usrdef_cflag}"

if [ "${var_usr_cmake_target}" = "Ninja" ]; then
	ninja -v
fi

cd ..

#if [ -d "${var_usr_cmake_mid_dir}" ]; then
#    rm -fr "${var_usr_cmake_mid_dir}"
#fi
