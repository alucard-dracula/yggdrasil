#!/bin/sh

var_local_dir=$(cd `dirname $0`; pwd)

var_projname=krb5
var_src_dir="${var_local_dir}/src"
var_perfix_root_dir="${var_local_dir}/stage"

rm -fr "${var_perfix_root_dir}"
mkdir -p "${var_perfix_root_dir}"

# macos
var_sdk_name=macosx 
var_sdk_ver_min=12.4
var_prefix_dir=${var_perfix_root_dir}/${var_projname}-${var_sdk_name}
var_host=x86_64-apple-darwin

var_build_dir=${var_local_dir}/src/build-macos-clang

rm -fr "${var_prefix_dir}"
mkdir -p "${var_prefix_dir}"

rm -fr "${var_build_dir}"
mkdir -p "${var_build_dir}"

cp -f "${var_local_dir}/build-krb5-darwin16_tpl.sh" "${var_build_dir}/build-krb5-darwin16_tpl.sh"

cd ${var_local_dir}/src
rm -f configure
autoreconf --verbose

cd "${var_build_dir}"

sh build-krb5-darwin16_tpl.sh \
    ${var_prefix_dir} \
    ${var_host} \
    ${var_sdk_name} \
    ${var_sdk_ver_min}

cd "${var_local_dir}"
