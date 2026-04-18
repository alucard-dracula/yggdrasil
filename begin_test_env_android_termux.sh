#!/bin/bash

var_local_dir=$(pwd)

echo "${var_local_dir}"

if [ "${LD_LIBRARY_PATH_TMP}" = "" ]; then
    PATH_TMP=${PATH}
    LD_LIBRARY_PATH_TMP=${LD_LIBRARY_PATH}
    export LD_LIBRARY_PATH_TMP
    export PATH_TMP
fi

var_python_dir=${var_local_dir}/../python
var_python_bin_dir=${var_python_dir}/bin
var_python_lib_dir=${var_python_dir}/lib

PATH=${var_local_dir}:${var_python_bin_dir}:${LD_LIBRARY_PATH_TMP}
LD_LIBRARY_PATH=${var_local_dir}:${var_python_lib_dir}:~/../usr/lib:${LD_LIBRARY_PATH_TMP}
ICU_DATA=${var_local_dir}
PYTHONHOME=${var_python_dir}
PYTHONPATH=${var_python_lib_dir}:${var_python_lib_dir}/python3.13:${var_python_lib_dir}/python3.13/site-packages

export PATH
export LD_LIBRARY_PATH
export ICU_DATA
export PYTHONHOME
export PYTHONPATH
