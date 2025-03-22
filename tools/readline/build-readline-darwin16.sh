#!/bin/sh

var_local_dir=$(cd `dirname $0`; pwd)

var_projname=readline
var_perfix_root_dir="${var_local_dir}/stage"

rm -fr "${var_perfix_root_dir}"
mkdir -p "${var_perfix_root_dir}"

# macos
var_sdk_name=macosx 
var_sdk_ver_min=12.4
var_prefix_dir=${var_perfix_root_dir}/${var_projname}-${var_sdk_name}
var_host=x86_64-apple-darwin

rm -fr "${var_prefix_dir}"
mkdir -p "${var_prefix_dir}"

sh build-readline-darwin16_tpl.sh \
    ${var_prefix_dir} \
    ${var_host} \
    ${var_sdk_name} \
    ${var_sdk_ver_min}

# iphoneos
var_sdk_name=iphoneos 
var_sdk_ver_min=15.6
var_prefix_dir=${var_perfix_root_dir}/${var_projname}-${var_sdk_name}
var_host=arm-apple-darwin

rm -fr "${var_prefix_dir}"
mkdir -p "${var_prefix_dir}"

sh build-readline-darwin16_tpl.sh \
    ${var_prefix_dir} \
    ${var_host} \
    ${var_sdk_name} \
    ${var_sdk_ver_min}

# iphonesimulator
var_sdk_name=iphonesimulator 
var_sdk_ver_min=15.6
var_prefix_dir=${var_perfix_root_dir}/${var_projname}-${var_sdk_name}
var_host=x86_64-apple-darwin

rm -fr "${var_prefix_dir}"
mkdir -p "${var_prefix_dir}"

sh build-readline-darwin16_tpl.sh \
    ${var_prefix_dir} \
    ${var_host} \
    ${var_sdk_name} \
    ${var_sdk_ver_min}
