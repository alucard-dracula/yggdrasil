#!/bin/sh

var_local_dir=$(cd `dirname $0`; pwd)

var_projname=libicu58

var_sdk_name_macosx=macosx
var_sdk_name_iphoneos=iphoneos
var_sdk_name_iphonesimulator=iphonesimulator

var_sdk_ver_min_mac=12.4
var_sdk_ver_min_ios=15.6

var_host_macosx=x86_64-apple-darwin
var_host_iphoneos=arm-apple-darwin
var_host_iphonesimulator=x86_64-apple-darwin

# build_dir
var_build_sh_files_dir=build_sh_files

var_build_dir_title=icu_darwin16_build

var_build_dir_macosx="${var_local_dir}/${var_build_dir_title}-${var_sdk_name_macosx}"
var_build_dir_iphoneos="${var_local_dir}/${var_build_dir_title}-${var_sdk_name_iphoneos}"
var_build_dir_iphonesimulator="${var_local_dir}/${var_build_dir_title}-${var_sdk_name_iphonesimulator}"

# perfix_dir
var_prefix_root_dir="${var_local_dir}/stage"

var_perfix_dir_macosx="${var_prefix_root_dir}/${var_projname}-${var_sdk_name_macosx}"
var_perfix_dir_iphoneos="${var_prefix_root_dir}/${var_projname}-${var_sdk_name_iphoneos}"
var_perfix_dir_iphonesimulator="${var_prefix_root_dir}/${var_projname}-${var_sdk_name_iphonesimulator}"

rm -fr "${var_prefix_root_dir}"

rm -fr "${var_build_dir_macosx}"
rm -fr "${var_build_dir_iphoneos}"
rm -fr "${var_build_dir_iphonesimulator}"

mkdir -p "${var_build_dir_macosx}"
mkdir -p "${var_build_dir_iphoneos}"
mkdir -p "${var_build_dir_iphonesimulator}"

mkdir -p "${var_perfix_dir_macosx}"
mkdir -p "${var_perfix_dir_iphoneos}"
mkdir -p "${var_perfix_dir_iphonesimulator}"

cp -f ${var_build_sh_files_dir}/icu4c_build_darwin16_tpl.sh ${var_build_dir_macosx}/
cp -f ${var_build_sh_files_dir}/icu4c_build_darwin16.sh ${var_build_dir_macosx}/

cp -f ${var_build_sh_files_dir}/icu4c_build_darwin16_tpl.sh ${var_build_dir_iphoneos}/
cp -f ${var_build_sh_files_dir}/icu4c_build_darwin16.sh ${var_build_dir_iphoneos}/

cp -f ${var_build_sh_files_dir}/icu4c_build_darwin16_tpl.sh ${var_build_dir_iphonesimulator}/
cp -f ${var_build_sh_files_dir}/icu4c_build_darwin16.sh ${var_build_dir_iphonesimulator}/

# macosx
cd ${var_build_dir_macosx}
sh icu4c_build_darwin16.sh \
    ${var_sdk_name_macosx} \
    ${var_sdk_ver_min_mac} \
    ${var_perfix_dir_macosx} \
    ${var_host_macosx} \
    ${var_build_dir_macosx}

cd ..

# iphoneos
cd ${var_build_dir_iphoneos}
sh icu4c_build_darwin16.sh \
    ${var_sdk_name_iphoneos} \
    ${var_sdk_ver_min_ios} \
    ${var_perfix_dir_iphoneos} \
    ${var_host_iphoneos} \
    ${var_build_dir_macosx}

cd ..

# iphonesimulator
cd ${var_build_dir_iphonesimulator}
sh icu4c_build_darwin16.sh \
    ${var_sdk_name_iphonesimulator} \
    ${var_sdk_ver_min_ios} \
    ${var_perfix_dir_iphonesimulator} \
    ${var_host_iphonesimulator} \
    ${var_build_dir_macosx}

cd ..
