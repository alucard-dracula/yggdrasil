#!/bin/sh

var_local_dir=$(cd `dirname $0`; pwd)

var_projname=boost

var_build_tmp_dir="${var_local_dir}/bin.v2"
var_perfix_root_dir="${var_local_dir}/stage_perfix"
var_stage_root_idr="${var_local_dir}/stage"

rm -fr "${var_perfix_root_dir}"
mkdir -p "${var_perfix_root_dir}"

rm -fr "${var_stage_root_idr}"
mkdir -p "${var_stage_root_idr}"

# macos
var_sdk_name=macosx 
var_sdk_ver_min=12.4
var_prefix_dir=${var_perfix_root_dir}/${var_projname}-${var_sdk_name}
var_stage_dir=${var_stage_root_idr}/${var_projname}-${var_sdk_name}

var_icu_root_path=~/third_part/libicu
var_zlib_source_path="${var_local_dir}/../zlib-1.3"
var_python_root_path=~/third_part/python
var_python_ver=3.13

var_mpi_root_dir=~/third_part/open-mpi

rm -fr "${var_prefix_dir}"
mkdir -p "${var_prefix_dir}/include"

rm -fr "${var_stage_dir}"
mkdir -p "${var_stage_dir}/lib"

rm -fr "${var_build_tmp_dir}"

export USRDEF_PYTHON_ROOT_PATH="${var_python_root_path}"
export USRDEF_PYTHON_VER="${var_python_ver}"
export USRDEF_FORCE_HAS_ICU="no"

# OpenMPI less arm lib and yggr is not used, so turned off it.
#export USRDEF_MPI_ROOT_PATH="${var_mpi_root_dir}"

sh build64-boost-darwin16_tpl.sh \
    ${var_prefix_dir} \
    ${var_stage_dir} \
    ${var_sdk_name} \
    ${var_sdk_ver_min} \
    ${var_icu_root_path} \
    ${var_zlib_source_path}

cp -fr "${var_stage_dir}/lib" "${var_prefix_dir}/"
cp -fr "${var_local_dir}/boost" "${var_prefix_dir}/include/"

# iphoneos
var_sdk_name=iphoneos 
var_sdk_ver_min=15.6
var_prefix_dir=${var_perfix_root_dir}/${var_projname}-${var_sdk_name}
var_stage_dir=${var_stage_root_idr}/${var_projname}-${var_sdk_name}

var_icu_root_path=~/third_part_iphoneos/libicu
var_zlib_source_path="${var_local_dir}/../zlib-1.3"
#var_python_root_path="~/third_part_iphoneos/python"
#var_python_ver=3.13

var_mpi_root_dir=""

rm -fr "${var_prefix_dir}"
mkdir -p "${var_prefix_dir}/include"

rm -fr "${var_stage_dir}"
mkdir -p "${var_stage_dir}/lib"

rm -fr "${var_build_tmp_dir}"

export USRDEF_PYTHON_ROOT_PATH=""
export USRDEF_PYTHON_VER=""
export USRDEF_FORCE_HAS_ICU="yes"

# OpenMPI less arm lib and yggr is not used, so turned off it.
#export USRDEF_MPI_ROOT_PATH="${var_mpi_root_dir}"

sh build64-boost-darwin16_tpl.sh \
    ${var_prefix_dir} \
    ${var_stage_dir} \
    ${var_sdk_name} \
    ${var_sdk_ver_min} \
    ${var_icu_root_path} \
    ${var_zlib_source_path}

cp -fr "${var_stage_dir}/lib" "${var_prefix_dir}/"
cp -fr "${var_local_dir}/boost" "${var_prefix_dir}/include/"

# iphonesimulator
var_sdk_name=iphonesimulator 
var_sdk_ver_min=15.6
var_prefix_dir=${var_perfix_root_dir}/${var_projname}-${var_sdk_name}
var_stage_dir=${var_stage_root_idr}/${var_projname}-${var_sdk_name}

var_icu_root_path=~/third_part_iphonesimulator/libicu
var_zlib_source_path="${var_local_dir}/../zlib-1.3"
#var_python_root_path="~/third_part_iphoneos/python"
#var_python_ver=3.13

var_mpi_root_dir=""

rm -fr "${var_prefix_dir}"
mkdir -p "${var_prefix_dir}/include"

rm -fr "${var_stage_dir}"
mkdir -p "${var_stage_dir}/lib"

rm -fr "${var_build_tmp_dir}"

export USRDEF_PYTHON_ROOT_PATH=""
export USRDEF_PYTHON_VER=""
export USRDEF_FORCE_HAS_ICU="yes"

# OpenMPI less arm lib and yggr is not used, so turned off it.
#export USRDEF_MPI_ROOT_PATH="${var_mpi_root_dir}"

sh build64-boost-darwin16_tpl.sh \
    ${var_prefix_dir} \
    ${var_stage_dir} \
    ${var_sdk_name} \
    ${var_sdk_ver_min} \
    ${var_icu_root_path} \
    ${var_zlib_source_path}

cp -fr "${var_stage_dir}/lib" "${var_prefix_dir}/"
cp -fr "${var_local_dir}/boost" "${var_prefix_dir}/include/"
