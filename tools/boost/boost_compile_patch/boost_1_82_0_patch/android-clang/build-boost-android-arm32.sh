#!/bin/bash

var_local_dir=$(cd `dirname $0`; pwd)

var_android_api=21
AndroidTargetVersion=${var_android_api}
AndroidTarget=armv7a-linux-androideabi
var_target_os=android
var_tool_set=clang-armeabiv7a
#var_python_ver=313
var_python_ver_float=3.13
#var_python_ver_float_full=3.13.13

export AndroidNDKRoot=/d/android_devel/Android/Sdk/ndk/current
export AndroidBinariesPath=${AndroidNDKRoot}/toolchains/llvm/prebuilt/windows-x86_64/bin
export AndroidTargetVersion64=${var_android_api}
export AndroidTargetVersion32=${var_android_api}
export NO_BZIP2=1
export PlatformOS=windows

export AndroidPythonTargetVersion=${var_android_api}
export PythonAndroidTarget=${AndroidTarget}
export AndroidPythonRootDir=${AndroidNDKRoot}/../../../../ndk_third_part_local/python
export PythonAndroidVersionFloat=${var_python_ver_float}

boost_third_part_dir=${var_local_dir}/third_part

icu_dir=${AndroidNDKRoot}/../../../../ndk_third_part_local/libicu
icu_boost_dir=${boost_third_part_dir}/libicu_boost

zlib_source_dir=${var_local_dir}/../zlib-1.3

rm -fr ${icu_boost_dir}

mkdir -p ${icu_boost_dir}

ln -s ${icu_dir}/include ${icu_boost_dir}/include
ln -s ${icu_dir}/lib/${AndroidTarget}/${AndroidTargetVersion} ${icu_boost_dir}/lib

#rm -fr "bin.v2"
#rm -fr stage

./b2 \
    --ignore-site-config \
    -j12 \
    target-os=${var_target_os} \
    toolset=${var_tool_set} \
    -sZLIB_SOURCE=${zlib_source_dir} \
    -sICU_PATH=${icu_boost_dir} \
    -sICU_ICUUC_NAME=icuuc \
    -sICU_ICUDT_NAME=icudata \
    -sICU_ICUIN_NAME=icui18n \
    --build-type=complete \
    address-model=32

