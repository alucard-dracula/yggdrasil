#!/bin/sh

var_android_api=21
AndroidTargetVersion=${var_android_api}
AndroidTarget=x86_64-linux-android
var_target_os=android
var_tool_set=clang-x86_64

export AndroidNDKRoot=/d/android_devel/Android/Sdk/ndk/28.0.12674087
export AndroidBinariesPath=${AndroidNDKRoot}/toolchains/llvm/prebuilt/windows-x86_64/bin
export AndroidTargetVersion64=${var_android_api}
export AndroidTargetVersion32=${var_android_api}
export NO_BZIP2=1
export PlatformOS=windows

export AndroidPythonTargetVersion=${var_android_api}
export PythonAndroidTarget=${AndroidTarget}
export AndroidPythonRootDir=${AndroidNDKRoot}/../../../../ndk_third_part_local/python3.12.1

icu_dir=${AndroidNDKRoot}/../../../../ndk_third_part_local/libicu
boost_thread_part_dir=thrid_part
icu_boost_dir=${boost_thread_part_dir}/libicu_boost

rm -fr ${icu_boost_dir}

mkdir -p ${icu_boost_dir}

ln -s ${icu_dir}/include ${icu_boost_dir}/include
ln -s ${icu_dir}/lib/${AndroidTarget}/${AndroidTargetVersion} ${icu_boost_dir}/lib

#rm -fr "bin.v2"
#rm -fr stage

sed -i 's/#<compileflags>-fno-integrated-as/<compileflags>-fno-integrated-as/g' ./tools/build/src/user-config.jam

./b2 \
    --with-context \
    --ignore-site-config \
    -j12 \
    target-os=${var_target_os} \
    toolset=${var_tool_set} \
    -sZLIB_SOURCE=/e/devel/build_tmp/android_build/zlib-1.3 \
    -sICU_PATH=${icu_boost_dir} \
    -sICU_ICUUC_NAME=icuuc \
    -sICU_ICUDT_NAME=icudata \
    -sICU_ICUIN_NAME=icui18n \
    --build-type=complete

sed -i 's/<compileflags>-fno-integrated-as/#<compileflags>-fno-integrated-as/g' ./tools/build/src/user-config.jam

./b2 \
    --with-context \
    --ignore-site-config \
    -j12 \
    target-os=${var_target_os} \
    toolset=${var_tool_set} \
    -sZLIB_SOURCE=/e/devel/build_tmp/android_build/zlib-1.3 \
    -sICU_PATH=${icu_boost_dir} \
    -sICU_ICUUC_NAME=icuuc \
    -sICU_ICUDT_NAME=icudata \
    -sICU_ICUIN_NAME=icui18n \
    --build-type=complete



