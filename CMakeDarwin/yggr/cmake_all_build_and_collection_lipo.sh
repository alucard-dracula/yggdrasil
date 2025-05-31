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

	if [ "${var_sdk_name}" = "iphoneos" ]; then
		var_sh_file="cmake-build-${var_sdk_name}-${var_clang_tag}-${var_cpp_ver}.sh"
		#	echo "sh ${var_sh_file}"
		sh "${var_sh_file}"

		if [ -d "${var_src_bin_dir}" ]; then
			cp -fr "${var_src_bin_dir}" "${var_prefix_sln_bin_dir}"
		fi

		if [ -d "${var_src_lib_dir}" ]; then
			cp -fr "${var_src_lib_dir}" "${var_prefix_sln_dir}/"
		fi
	else
		mkdir -p "${var_prefix_sln_bin_dir}"
		mkdir -p "${var_prefix_sln_lib_dir}"

		#x86_64
		var_prefix_sln_bin_dir_x86_64="${var_prefix_sln_bin_dir}/x86_64"
		var_prefix_sln_lib_dir_x86_64="${var_prefix_sln_lib_dir}/x86_64"

		var_sh_file_x86_64="cmake-build-${var_sdk_name}-${var_clang_tag}-${var_cpp_ver}-x86_64.sh"
		#	echo "sh ${var_sh_file}"
		sh "${var_sh_file_x86_64}"

		if [ -d "${var_src_bin_dir}" ]; then
			cp -fr "${var_src_bin_dir}" "${var_prefix_sln_bin_dir_x86_64}"
		fi

		if [ -d "${var_src_lib_dir}" ]; then
			cp -fr "${var_src_lib_dir}" "${var_prefix_sln_lib_dir_x86_64}/"
		fi

		#arm64
		var_prefix_sln_bin_dir_arm64="${var_prefix_sln_bin_dir}/arm64"
		var_prefix_sln_lib_dir_arm64="${var_prefix_sln_lib_dir}/arm64"

		var_sh_file_arm64="cmake-build-${var_sdk_name}-${var_clang_tag}-${var_cpp_ver}-arm64.sh"
		#	echo "sh ${var_sh_file}"
		sh "${var_sh_file_arm64}"

		if [ -d "${var_src_bin_dir}" ]; then
			cp -fr "${var_src_bin_dir}" "${var_prefix_sln_bin_dir_arm64}"
		fi

		if [ -d "${var_src_lib_dir}" ]; then
			cp -fr "${var_src_lib_dir}" "${var_prefix_sln_lib_dir_arm64}/"
		fi

		var_x86_exe_list=$(find ${var_prefix_sln_bin_dir_x86_64} -type f -perm +555)
		echo "${var_x86_exe_list}"

		for var_fpath in ${var_x86_exe_list}
		do
			var_fname=${var_fpath##*/}

			if [ -f "${var_prefix_sln_bin_dir_arm64}/${var_fname}" ]; then
				var_cmd_create="lipo -create ${var_fpath} ${var_prefix_sln_bin_dir_arm64}/${var_fname} -output ${var_prefix_sln_bin_dir}/${var_fname}"
				var_cmd_check="lipo -info ${var_prefix_sln_bin_dir}/${var_fname}"
				echo "${var_cmd_create}"
				eval ${var_cmd_create}
				eval ${var_cmd_check}
			fi
		done

		var_x86_so_list=$(find ${var_prefix_sln_lib_dir_x86_64} -type f -name '*.so')
		echo "${var_x86_so_list}"

		for var_fpath in ${var_x86_so_list}
		do
		var_fname=${var_fpath##*/}

			if [ -f "${var_prefix_sln_lib_dir_arm64}/${var_fname}" ]; then
				var_cmd_create="lipo -create ${var_fpath} ${var_prefix_sln_lib_dir_arm64}/${var_fname} -output ${var_prefix_sln_lib_dir}/${var_fname}"
				var_cmd_check="lipo -info ${var_prefix_sln_lib_dir}/${var_fname}"
				echo "${var_cmd_create}"
				eval ${var_cmd_create}
				eval ${var_cmd_check}
			fi
		done

		var_x86_dylib_list=$(find ${var_prefix_sln_lib_dir_x86_64} -type f -name '*.dylib')
		echo "${var_x86_dylib_list}"

		for var_fpath in ${var_x86_dylib_list}
		do
		var_fname=${var_fpath##*/}

			if [ -f "${var_prefix_sln_lib_dir_arm64}/${var_fname}" ]; then
				var_cmd_create="lipo -create ${var_fpath} ${var_prefix_sln_lib_dir_arm64}/${var_fname} -output ${var_prefix_sln_lib_dir}/${var_fname}"
				var_cmd_check="lipo -info ${var_prefix_sln_lib_dir}/${var_fname}"
				echo "${var_cmd_create}"
				eval ${var_cmd_create}
				eval ${var_cmd_check}
			fi
		done

		var_x86_a_list=$(find ${var_prefix_sln_lib_dir_x86_64} -type f -name '*.a')
		echo "${var_x86_a_list}"

		for var_fpath in ${var_x86_a_list}
		do
			var_fname=${var_fpath##*/}

			if [ -f "${var_prefix_sln_lib_dir_arm64}/${var_fname}" ]; then
				var_cmd_create="lipo -create ${var_fpath} ${var_prefix_sln_lib_dir_arm64}/${var_fname} -output ${var_prefix_sln_lib_dir}/${var_fname}"
				var_cmd_check="lipo -info ${var_prefix_sln_lib_dir}/${var_fname}"
				echo "${var_cmd_create}"
				eval ${var_cmd_create}
				eval ${var_cmd_check}
			fi
		done
	fi

	if [ -d "${var_src_inc_dir}" ]; then
		cp -fr "${var_src_inc_dir}" "${var_prefix_sln_dir}/"
	fi
}

var_sln_name=yggr
var_clang_tag="clang-darwin16"
var_cpp_ver="cpp11"

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

