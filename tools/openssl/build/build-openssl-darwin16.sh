#!/bin/sh

var_local_dir=$(cd `dirname $0`; pwd)

var_projname=openssl_3.4.0
var_perfix_root_dir="${var_local_dir}/stage"

rm -fr "${var_perfix_root_dir}"
mkdir -p "${var_perfix_root_dir}"

# macos
#var_ssl_target=darwin64-x86_64
#var_ssl_target=darwin64-arm64
var_sdk_name=macosx 
var_sdk_ver_min=12.4

var_prefix_dir_result=${var_perfix_root_dir}/${var_projname}-${var_sdk_name}

rm -fr "${var_prefix_dir_result}"

# x86_64
var_prefix_dir_x86_64=${var_perfix_root_dir}/${var_projname}-${var_sdk_name}-x86_64
var_prefix_dir=${var_prefix_dir_x86_64}
var_ssl_target=darwin64-x86_64

var_openssl_dir="${var_prefix_dir}/ssl"


rm -fr "${var_prefix_dir}"
mkdir -p "${var_openssl_dir}"

rm -f Makefile

sh build-openssl-darwin16_tpl.sh \
    ${var_prefix_dir} \
    ${var_openssl_dir} \
    ${var_ssl_target} \
    ${var_sdk_name} \
    ${var_sdk_ver_min} 

#arm64
var_prefix_dir_arm64=${var_perfix_root_dir}/${var_projname}-${var_sdk_name}-arm64
var_prefix_dir=${var_prefix_dir_arm64}
var_ssl_target=darwin64-arm64

var_openssl_dir="${var_prefix_dir}/ssl"

rm -fr "${var_prefix_dir}"
mkdir -p "${var_openssl_dir}"

rm -f Makefile

sh build-openssl-darwin16_tpl.sh \
    ${var_prefix_dir} \
    ${var_openssl_dir} \
    ${var_ssl_target} \
    ${var_sdk_name} \
    ${var_sdk_ver_min} 

#lipo merge
sh build-openssl-darwin16_merge.sh \
    ${var_prefix_dir_arm64} \
    ${var_prefix_dir_x86_64} \
    ${var_prefix_dir_result} \
    "x86_64" 

if [ ! -l "${var_prefix_dir_result}/lib/libcrypto.dylib" ]; then
    ln -s "libcrypto.3.dylib" "${var_prefix_dir_result}/lib/libcrypto.dylib"
fi

if [ ! -l "${var_prefix_dir_result}/lib/libssl.dylib" ]; then
    ln -s "libssl.3.dylib" "${var_prefix_dir_result}/lib/libcrypto.dylib"
fi

# iphoneos
# ios64-xcrun
var_sdk_name=iphoneos 
var_sdk_ver_min=15.6

var_prefix_dir_result=${var_perfix_root_dir}/${var_projname}-${var_sdk_name}

rm -fr "${var_prefix_dir_result}"

# arm64
var_prefix_dir=${var_prefix_dir_result}
var_ssl_target=ios64-xcrun

var_openssl_dir="${var_prefix_dir}/ssl"


rm -fr "${var_prefix_dir}"
mkdir -p "${var_openssl_dir}"

rm -f Makefile

sh build-openssl-darwin16_tpl.sh \
    ${var_prefix_dir} \
    ${var_openssl_dir} \
    ${var_ssl_target} \
    ${var_sdk_name} \
    ${var_sdk_ver_min} 

# iphonesimulator
#iossimulator-arm64-xcrun
#iossimulator-x86_64-xcrun
var_sdk_name=iphonesimulator 
var_sdk_ver_min=15.6

var_prefix_dir_result=${var_perfix_root_dir}/${var_projname}-${var_sdk_name}

rm -fr "${var_prefix_dir_result}"

# x86_64
var_prefix_dir_x86_64=${var_perfix_root_dir}/${var_projname}-${var_sdk_name}-x86_64
var_prefix_dir=${var_prefix_dir_x86_64}
var_ssl_target=iossimulator-x86_64-xcrun

var_openssl_dir="${var_prefix_dir}/ssl"


rm -fr "${var_prefix_dir}"
mkdir -p "${var_openssl_dir}"

rm -f Makefile

sh build-openssl-darwin16_tpl.sh \
    ${var_prefix_dir} \
    ${var_openssl_dir} \
    ${var_ssl_target} \
    ${var_sdk_name} \
    ${var_sdk_ver_min} 

#arm64
var_prefix_dir_arm64=${var_perfix_root_dir}/${var_projname}-${var_sdk_name}-arm64
var_prefix_dir=${var_prefix_dir_arm64}
var_ssl_target=iossimulator-arm64-xcrun

var_openssl_dir="${var_prefix_dir}/ssl"

rm -fr "${var_prefix_dir}"
mkdir -p "${var_openssl_dir}"

rm -f Makefile

sh build-openssl-darwin16_tpl.sh \
    ${var_prefix_dir} \
    ${var_openssl_dir} \
    ${var_ssl_target} \
    ${var_sdk_name} \
    ${var_sdk_ver_min} 

#lipo merge
sh build-openssl-darwin16_merge.sh \
    ${var_prefix_dir_arm64} \
    ${var_prefix_dir_x86_64} \
    ${var_prefix_dir_result} \
    "x86_64" 

