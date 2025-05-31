#!/bin/bash

var_local_dir=$(cd `dirname $0`; pwd)

var_projname=cyrus-sasl
var_projver="2.1.28"

# macos
var_sdk_name=macosx 
var_sdk_ver_min=12.4
#var_prefix_dir=/Users/alucard/third_part/${var_projname}-${var_sdk_name}
var_prefix_dir=/Users/alucard/third_part/${var_projname}-${var_projver}
var_host=x86_64-apple-darwin

var_build_dir="${var_local_dir}/build-macosx-clang"

rm -fr "${var_build_dir}"
mkdir -p "${var_build_dir}"

rm -fr "${var_prefix_dir}"
mkdir -p "${var_prefix_dir}"

ln -s "${var_local_dir}/build-cyrus-sasl2-darwin16_tpl.sh" "${var_build_dir}/build-cyrus-sasl2-darwin16_tpl.sh"

cd "${var_build_dir}"

sh build-cyrus-sasl2-darwin16_tpl.sh \
    ${var_prefix_dir} \
    ${var_host} \
    ${var_sdk_name} \
    ${var_sdk_ver_min}

cd "${var_local_dir}"

# iphoneos
var_sdk_name=iphoneos 
var_sdk_ver_min=15.6
#var_prefix_dir=${var_perfix_root_dir}/${var_projname}-${var_sdk_name}
var_prefix_dir=/Users/alucard/third_part_iphoneos/${var_projname}-${var_projver}
var_host=arm-apple-darwin

var_build_dir="${var_local_dir}/build-iphoneos-clang"

rm -fr "${var_build_dir}"
mkdir -p "${var_build_dir}"

rm -fr "${var_prefix_dir}"
mkdir -p "${var_prefix_dir}"

ln -s "${var_local_dir}/build-cyrus-sasl2-darwin16_tpl.sh" "${var_build_dir}/build-cyrus-sasl2-darwin16_tpl.sh"

cd "${var_build_dir}"

sh build-cyrus-sasl2-darwin16_tpl.sh \
    ${var_prefix_dir} \
    ${var_host} \
    ${var_sdk_name} \
    ${var_sdk_ver_min}

cd "${var_local_dir}"

# iphonesimulator
var_sdk_name=iphonesimulator 
var_sdk_ver_min=15.6
#var_prefix_dir=${var_perfix_root_dir}/${var_projname}-${var_sdk_name}
var_prefix_dir=/Users/alucard/third_part_iphonesimulator/${var_projname}-${var_projver}
var_host=x86_64-apple-darwin

var_build_dir="${var_local_dir}/build-iphonesimulator-clang"

rm -fr "${var_build_dir}"
mkdir -p "${var_build_dir}"

rm -fr "${var_prefix_dir}"
mkdir -p "${var_prefix_dir}"

ln -s "${var_local_dir}/build-cyrus-sasl2-darwin16_tpl.sh" "${var_build_dir}/build-cyrus-sasl2-darwin16_tpl.sh"

cd "${var_build_dir}"

sh build-cyrus-sasl2-darwin16_tpl.sh \
    ${var_prefix_dir} \
    ${var_host} \
    ${var_sdk_name} \
    ${var_sdk_ver_min}

cd "${var_local_dir}"
