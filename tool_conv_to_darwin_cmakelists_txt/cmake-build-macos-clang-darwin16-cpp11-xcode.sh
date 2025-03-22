#!/bin/bash

var_usr_cmake_cc=default
var_usr_cmake_cxx=default

#var_usr_cmake_cc=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang
#var_usr_cmake_cxx=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++

var_usr_cmake_compiler_version=clang-darwin16

var_usr_cmake_c_standard=99
var_usr_cmake_cxx_standard=11

echo ${var_usr_cmake_mid_dir}

# 64bit debug release
var_usr_cmake_arch_bits=64

var_usr_cmake_build_type=Debug
var_usr_cmake_mid_dir=cmake_build_cache-xcode-debug

var_usr_cmake_lib_root_dir=lib

sh cmake-build-tpl-xcode.sh \
    ${var_usr_cmake_cc} \
    ${var_usr_cmake_cxx} \
    ${var_usr_cmake_compiler_version} \
    ${var_usr_cmake_c_standard} \
    ${var_usr_cmake_cxx_standard} \
    ${var_usr_cmake_arch_bits} \
    ${var_usr_cmake_build_type} \
    ${var_usr_cmake_mid_dir} 

var_usr_cmake_build_type=Release
var_usr_cmake_mid_dir=cmake_build_cache-xcode-release

sh cmake-build-tpl-xcode.sh \
    ${var_usr_cmake_cc} \
    ${var_usr_cmake_cxx} \
    ${var_usr_cmake_compiler_version} \
    ${var_usr_cmake_c_standard} \
    ${var_usr_cmake_cxx_standard} \
    ${var_usr_cmake_arch_bits} \
    ${var_usr_cmake_build_type} \
    ${var_usr_cmake_mid_dir} 

# xcode10 and later not support 32bit abi
