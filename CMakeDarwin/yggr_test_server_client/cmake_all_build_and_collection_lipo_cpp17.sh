#!/bin/bash

var_local_dir=$(cd `dirname $0`; pwd)

collection_foo(){ 
	var_sln_name=$1
	var_sdk_name=$2
	var_clang_tag=$3
	var_cpp_ver=$4
	var_src_root_dir=$5
	var_prefix_stage_dir=$6

#	echo "var_sln_name = ${var_sln_name}"
#	echo "var_sdk_name = ${var_sdk_name}"
#	echo "var_clang_tag = ${var_clang_tag}"
#	echo "var_cpp_ver = ${var_cpp_ver}"
#	echo "var_src_root_dir = ${var_src_root_dir}"
#	echo "var_prefix_stage_dir = ${var_prefix_stage_dir}"

	var_sh_file="cmake-build-${var_sdk_name}-${var_clang_tag}-${var_cpp_ver}.sh"

	var_src_bin_dir="${var_src_root_dir}/Release-x64/${var_clang_tag}"
	var_src_lib_dir="${var_src_root_dir}/lib"
	var_src_inc_dir="${var_src_root_dir}/include"

	var_prefix_sln_dir="${var_prefix_stage_dir}/${var_sln_name}-${var_sdk_name}"
	var_prefix_sln_bin_dir="${var_prefix_sln_dir}/bin"
	var_prefix_sln_lib_dir="${var_prefix_sln_dir}/lib"
	var_prefix_sln_inc_dir="${var_prefix_sln_dir}/include"

	if [ -d "${var_prefix_sln_dir}" ]; then
		rm -fr "${var_prefix_sln_dir}"
	fi

	if [ -d "${var_src_inc_dir}" ]; then
		mkdir -p "${var_prefix_sln_dir}"
	else
		mkdir -p "${var_prefix_sln_inc_dir}"
	fi

#	echo "sh ${var_sh_file}"
	sh "${var_sh_file}"

	if [ -d "${var_src_bin_dir}" ]; then
		cp -fr "${var_src_bin_dir}" "${var_prefix_sln_bin_dir}"
	fi

	if [ -d "${var_src_lib_dir}" ]; then
		cp -fr "${var_src_lib_dir}" "${var_prefix_sln_dir}/"
	fi

	if [ -d "${var_src_inc_dir}" ]; then
		cp -fr "${var_src_inc_dir}" "${var_prefix_sln_dir}/"
	fi
}

var_sln_name=yggr_test_server_client
var_clang_tag="clang-darwin16"
var_cpp_ver="cpp17"

var_sln_dir="${var_local_dir}/../.."
var_prefix_stage_dir="${var_sln_dir}/stage_prefix"

var_sdk_name="macosx"
collection_foo \
	"${var_sln_name}" \
	"${var_sdk_name}" \
	"${var_clang_tag}" \
	"${var_cpp_ver}" \
	"${var_sln_dir}" \
	"${var_prefix_stage_dir}" 

var_sdk_name="iphoneos"

collection_foo \
	"${var_sln_name}" \
	"${var_sdk_name}" \
	"${var_clang_tag}" \
	"${var_cpp_ver}" \
	"${var_sln_dir}" \
	"${var_prefix_stage_dir}" 

var_sdk_name="iphonesimulator"

collection_foo \
	"${var_sln_name}" \
	"${var_sdk_name}" \
	"${var_clang_tag}" \
	"${var_cpp_ver}" \
	"${var_sln_dir}" \
	"${var_prefix_stage_dir}" 

