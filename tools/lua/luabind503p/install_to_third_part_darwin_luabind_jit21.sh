#!/bin/bash

install_one_foo() {
    var_local_dir=$1
    var_third_part_root_dir=$2
    var_sdk_name=$3
    var_proj_name=$4
    var_prefix_name=$5

    var_third_part_dir="${var_third_part_root_dir}/third_part_${var_sdk_name}"

    var_prefix_dir="${var_third_part_dir}/${var_proj_name}-${var_sdk_name}"
    var_ln_dir_ver="${var_third_part_dir}/${var_prefix_name}"
    var_ln_dir="${var_third_part_dir}/${var_prefix_name:0:7}"
    var_src_dir="${var_local_dir}/stage_prefix/${var_proj_name}-${var_sdk_name}"

    if [ -d "${var_prefix_dir}" ]; then
#       echo "rm -fr ${var_prefix_dir}"
        rm -fr "${var_prefix_dir}"
        rm -f "${var_ln_dir_ver}"
        #rm -f "${var_ln_dir}"
    fi

#   include fix
#   bin_dir fix
    if [ -d "${var_src_dir}/bin" ]; then
        rm -fr "${var_src_dir}/bin"
    fi


    echo "cp -fr ${var_src_dir} ${var_prefix_dir}"
    echo "ln -s ${var_proj_name}-${var_sdk_name} ${var_ln_dir_ver}"
    #echo "ln -s ${var_proj_name}-${var_sdk_name} ${var_ln_dir}"
    cp -fr "${var_src_dir}" "${var_prefix_dir}"
    ln -s ${var_proj_name}-${var_sdk_name} ${var_ln_dir_ver}
    #ln -s ${var_proj_name}-${var_sdk_name} ${var_ln_dir}
}

var_local_dir=$(cd `dirname $0`; pwd)

var_proj_name="luabind_jit21"
var_third_part_root_dir="/Users/${USER}"
var_prefix_name="luabind_jit21"

var_sdk_name=macosx

install_one_foo \
    "${var_local_dir}" \
    "${var_third_part_root_dir}" \
    "${var_sdk_name}" \
    "${var_proj_name}" \
    "${var_prefix_name}"
