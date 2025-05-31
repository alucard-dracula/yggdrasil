#!/bin/sh

var_ndk_root=/d/android_devel/Android/Sdk/ndk/current
var_ndk_build_cmd=${var_ndk_root}/ndk-build.cmd

var_android_api=21
var_outdir_root=./stage
var_middir_root=./stage/objs
var_middir_local=${var_middir_root}/local
var_outdir=${var_outdir_root}/yggr
var_outdir_include=${var_outdir}/include
var_outdir_lib=${var_outdir}/lib

rm -fr ${var_outdir_root}

mkdir -p "${var_outdir_include}"
mkdir -p "${var_outdir_lib}"

#${var_ndk_build_cmd} -j12 NDK_DEBUG=1 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_yggr_lib_only_cpp20.mk APP_BUILD_SCRIPT=jni/Android_yggr_lib_only_a.mk V=1 > yggr_jni.log 2>&1
${var_ndk_build_cmd} -j12 NDK_DEBUG=1 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_yggr_lib_only_cpp20.mk APP_BUILD_SCRIPT=jni/Android_yggr_lib_only_a.mk
${var_ndk_build_cmd} -j12 NDK_DEBUG=0 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_yggr_lib_only_cpp20.mk APP_BUILD_SCRIPT=jni/Android_yggr_lib_only_a.mk

find ${var_middir_local} -type d -name 'objs*' -exec rm -fr {} \;

cp -fr ${var_middir_local}/* ${var_outdir_lib}/

${var_ndk_build_cmd} -j12 NDK_DEBUG=1 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_yggr_lib_only_cpp20.mk APP_BUILD_SCRIPT=jni/Android_yggr_lib_only_so.mk
${var_ndk_build_cmd} -j12 NDK_DEBUG=0 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_yggr_lib_only_cpp20.mk APP_BUILD_SCRIPT=jni/Android_yggr_lib_only_so.mk

#${var_ndk_build_cmd} -j12 NDK_DEBUG=1 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_yggr_lib_only_cpp20.mk APP_BUILD_SCRIPT=jni/Android_yggr_lib_only_exe.mk
#${var_ndk_build_cmd} -j12 NDK_DEBUG=0 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_yggr_lib_only_cpp20.mk APP_BUILD_SCRIPT=jni/Android_yggr_lib_only_exe.mk

touch ${var_outdir_lib}/arm64-v8a/mark-arm64-v8a.txt
touch ${var_outdir_lib}/armeabi-v7a/mark-armeabi-v7a.txt
touch ${var_outdir_lib}/riscv64/mark-riscv64.txt
touch ${var_outdir_lib}/x86_64/mark-x86_64.txt
touch ${var_outdir_lib}/x86/mark-x86.txt

cp -f jni_lib_format.bat ${var_outdir}/