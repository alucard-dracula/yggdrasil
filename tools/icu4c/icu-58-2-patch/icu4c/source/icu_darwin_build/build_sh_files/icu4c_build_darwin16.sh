#!/bin/sh

var_local_dir=$(cd `dirname $0`; pwd)

var_sdk_name=$1
var_sdk_ver_min=$2
var_prefix_dir=$3
var_host=$4
var_cross_build_dir=$5

sh icu4c_build_darwin16_tpl.sh \
    ${var_prefix_dir} \
    ${var_host} \
    ${var_sdk_name} \
    ${var_sdk_ver_min} \
    ${var_cross_build_dir}
