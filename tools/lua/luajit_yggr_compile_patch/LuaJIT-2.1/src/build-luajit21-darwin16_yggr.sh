#!/bin/sh

#luajit not support iOS

var_local_dir=$(cd `dirname $0`; pwd)

var_projname=luajit21
var_perfix_root_dir="${var_local_dir}/stage"

rm -fr "${var_perfix_root_dir}"
mkdir -p "${var_perfix_root_dir}"

var_src_sys=Darwin
var_src_arch=x86_64

# macos
var_sdk_name=macosx 
var_sdk_ver_min=12.4
var_prefix_dir=${var_perfix_root_dir}/${var_projname}-${var_sdk_name}

# x86_dst
var_dst_sys=Darwin
var_dst_arch=x86_64
var_prefix_dir_x86_64=${var_prefix_dir}-${var_dst_arch}

rm -fr "${var_prefix_dir_x86_64}"
mkdir -p "${var_prefix_dir_x86_64}/bin"
mkdir -p "${var_prefix_dir_x86_64}/lib"

sh build-luajit21-darwin16_yggr_tpl.sh \
    ${var_prefix_dir_x86_64} \
    ${var_sdk_name} \
    ${var_sdk_ver_min} \
    ${var_src_sys} \
    ${var_src_arch} \
    ${var_dst_sys} \
    ${var_dst_arch}

# arm64_dst
var_dst_arch=arm64
var_prefix_dir_arm64=${var_prefix_dir}-${var_dst_arch}

rm -fr "${var_prefix_dir_arm64}"
mkdir -p "${var_prefix_dir_arm64}/bin"
mkdir -p "${var_prefix_dir_arm64}/lib"

sh build-luajit21-darwin16_yggr_tpl.sh \
    ${var_prefix_dir_arm64} \
    ${var_sdk_name} \
    ${var_sdk_ver_min} \
    ${var_src_sys} \
    ${var_src_arch} \
    ${var_dst_sys} \
    ${var_dst_arch}

var_src_root_dir_arm64=$1
var_src_root_dir_x86_64=$2
var_dst_root_dir=$3
var_dst_root_sample=$4

sh build-luajit21-darwin16_merge.sh \
    ${var_prefix_dir_arm64} \
    ${var_prefix_dir_x86_64} \
    ${var_prefix_dir} \
    "x86_64"


