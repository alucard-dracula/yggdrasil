#!/bin/bash

var_usr_cmake_cc=$1
var_usr_cmake_cxx=$2
var_usr_cmake_compiler_version=$3

var_usr_cmake_c_standard=$4
var_usr_cmake_cxx_standard=$5

var_usr_cmake_arch_bits=$6
var_usr_cmake_build_type=$7
var_usr_cmake_mid_dir=$8


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

if [ "${var_usr_cmake_arch_bits}" = "" ]; then
    var_usr_cmake_arch_bits=32
fi

var_usr_cmake_arch_bits_tag=-x${var_usr_cmake_arch_bits}


if [ ! "${var_usr_cmake_build_type}" = "Debug" ]; then
    var_usr_cmake_build_type="Release"
fi

var_usr_cmake_bin_root_dir=${var_usr_cmake_build_type}${var_usr_cmake_arch_bits_tag}
var_usr_cmake_bin_outdir=${var_usr_cmake_bin_root_dir}/${var_usr_cmake_compiler_version}

# macos clang not support 32 bit
#if [ "${var_usr_cmake_arch_bits}" = "64" ]; then
#    var_usr_cmake_lib_outdir=lib${var_usr_cmake_arch_bits}
#else
#    var_usr_cmake_lib_outdir=lib
#fi

var_usr_cmake_lib_outdir=lib

if [ -d "${var_usr_cmake_mid_dir}" ]; then
    rm -fr "${var_usr_cmake_mid_dir}"
fi

#if [ -d "${var_usr_cmake_bin_root_dir}" ]; then
#    rm -fr "${var_usr_cmake_bin_root_dir}"
#fi

#if [ -d "${var_usr_cmake_lib_outdir}" ]; then
#    rm -fr "${var_usr_cmake_lib_outdir}"
#fi

mkdir -p "${var_usr_cmake_mid_dir}"

cd ${var_usr_cmake_mid_dir}

cmake \
    -G Ninja \
    .. \
    -DUSRDEF_CMAKE_C_COMPILER=${var_usr_cmake_cc} \
    -DUSRDEF_CMAKE_CXX_COMPILER=${var_usr_cmake_cxx} \
    -DUSRDEF_CMAKE_COMPILER_VERSION=${var_usr_cmake_compiler_version} \
    -DUSRDEF_CMAKE_BIN_OUTDIR=${var_usr_cmake_bin_outdir} \
    -DUSRDEF_CMAKE_LIB_OUTDIR=${var_usr_cmake_lib_outdir} \
    -DUSRDEF_CMAKE_C_STANDARD=${var_usr_cmake_c_standard} \
    -DUSRDEF_CMAKE_CXX_STANDARD=${var_usr_cmake_cxx_standard} \
    -DUSRDEF_CMAKE_BUILD_TYPE=${var_usr_cmake_build_type} \
    -DUSRDEF_CMAKE_BUILD_ARCH_BITS=${var_usr_cmake_arch_bits}

ninja -v

cd ..

#if [ -d "${var_usr_cmake_mid_dir}" ]; then
#    rm -fr "${var_usr_cmake_mid_dir}"
#fi