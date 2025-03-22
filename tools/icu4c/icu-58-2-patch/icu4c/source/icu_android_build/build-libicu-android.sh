#!/bin/sh

var_local_dir=$(cd `dirname $0`; pwd)

var_tool_build_dir=icu_mingw_cross_build
var_android_build_dir=icu_android_cross_build
var_build_sh_files_dir=build_sh_files

rm -fr ${var_tool_build_dir}
rm -fr ${var_android_build_dir}

mkdir -p "${var_tool_build_dir}"
mkdir -p "${var_android_build_dir}"

cp -f ${var_build_sh_files_dir}/icu4c_build_mingw-cross_tpl.sh ${var_tool_build_dir}/
cp -f ${var_build_sh_files_dir}/icu4c_build_mingw-cross_x64.sh ${var_tool_build_dir}/

cp -f ${var_build_sh_files_dir}/build-libicu-android-cross_tpl.sh ${var_android_build_dir}/
cp -f ${var_build_sh_files_dir}/build-libicu-android-cross.sh ${var_android_build_dir}/

cd ${var_tool_build_dir}

sh icu4c_build_mingw-cross_x64.sh

cd ..

cd ${var_android_build_dir}

sh build-libicu-android-cross.sh "${var_local_dir}/${var_tool_build_dir}"

cd ..
