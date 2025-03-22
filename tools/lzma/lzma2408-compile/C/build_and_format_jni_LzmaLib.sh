#!/bin/sh

var_ndk_root=/d/android_devel/Android/Sdk/ndk/28.0.12674087
var_ndk_build_cmd=${var_ndk_root}/ndk-build.cmd

var_android_api=21
var_middir_root=obj
var_outdir_root=stage
var_outdir=${var_outdir_root}/LzmaLib
var_outdir_include=${var_outdir}/include
var_outdir_lib=${var_outdir}/lib

rm -fr ${var_middir_root}
rm -fr ${var_outdir_root}

${var_ndk_build_cmd} NDK_DEBUG=1
${var_ndk_build_cmd} NDK_DEBUG=0

find ./obj -type d -name 'objs*' -exec rm -fr {} \;

mkdir -p ${var_outdir_include}

cp -fr obj/local ${var_outdir_lib}

touch ${var_outdir_lib}/arm64-v8a/mark-arm64-v8a.txt
touch ${var_outdir_lib}/armeabi-v7a/mark-armeabi-v7a.txt
touch ${var_outdir_lib}/riscv64/mark-riscv64.txt
touch ${var_outdir_lib}/x86_64/mark-x86_64.txt
touch ${var_outdir_lib}/x86/mark-x86.txt

cp -f jni_lib_format.bat ${var_outdir}/