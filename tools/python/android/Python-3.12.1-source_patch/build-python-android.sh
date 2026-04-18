#!/bin/bash

var_local_dir=$(cd `dirname $0`; pwd)

AndroidNDK=/d/android_devel/Android/Sdk/ndk/current
AndroidThirdPartLocal=${AndroidNDK}/../../../../ndk_third_part_local

AndroidAPI=21
AndroidToolChainPlatfrom=windows-x86_64

NDK_CFLAGS="\
    -fPIC \
    -Dasm=__asm__ \
    -I${AndroidThirdPartLocal}/bzip2/include \
    -I${AndroidThirdPartLocal}/libb2/include \
    -I${AndroidThirdPartLocal}/liblzma_xz/include \
    -I${AndroidThirdPartLocal}/readline/include \
    -I${AndroidThirdPartLocal}/libffi/include \
    -I${AndroidThirdPartLocal}/libunwind/include \
    "

var_python_var_float_full=3.12.1
var_python_ver=${var_python_var_float_full:0:1}${var_python_var_float_full:2:2}
var_python_var_main=${var_python_var_float_full:0:1}
var_python_var_float=${var_python_var_float_full:0:4}

PythonToolChain=/d/Python${var_python_ver}/python

AndroidTarget_list="
    aarch64-linux-android
    armv7a-linux-androideabi
    x86_64-linux-android
    i686-linux-android
    riscv64-linux-android
    "

prefix_root_dir=${var_local_dir}/../python_stage

prefix_tmp_dir=${prefix_root_dir}/build_tmp
prefix_dir=${prefix_root_dir}/python${var_python_var_float_full}

prefix_dir_include=${prefix_dir}/include

rm -fr ${prefix_root_dir}
mkdir -p ${prefix_dir}
mkdir -p ${prefix_tmp_dir}

for var_target in ${AndroidTarget_list} 
do
    echo "make" ${var_target}

    if [ "${var_target}" = "riscv64-linux-android" ]; then
        if [ ${AndroidAPI} -ge 35 ]; then
            USED_AndroidAPI=${AndroidAPI}
        else
            USED_AndroidAPI=35
        fi
    else
        USED_AndroidAPI=${AndroidAPI}
    fi 

    NDK_LDFLAGS="\
        -L${AndroidThirdPartLocal}/bzip2/lib/${var_target}/${USED_AndroidAPI} \
        -L${AndroidThirdPartLocal}/libb2/lib/${var_target}/${USED_AndroidAPI} \
        -L${AndroidThirdPartLocal}/liblzma_xz/lib/${var_target}/${USED_AndroidAPI} \
        -L${AndroidThirdPartLocal}/readline/lib/${var_target}/${USED_AndroidAPI} \
        -L${AndroidThirdPartLocal}/libffi/lib/${var_target}/${USED_AndroidAPI} \
        -L${AndroidThirdPartLocal}/libunwind/lib/${var_target}/${USED_AndroidAPI} \
        "

    make clean
    sh build-python-android-tpl.sh \
        "${prefix_tmp_dir}" "${AndroidNDK}" "${var_target}" "${USED_AndroidAPI}" \
        "${AndroidToolChainPlatfrom}" "${NDK_CFLAGS}" "${NDK_LDFLAGS}" "${PythonToolChain}"
    
    if [ ! -d ${prefix_dir_include} ]; then
        cp -fr ${prefix_tmp_dir}/include/python${var_python_var_float} ${prefix_dir}/include
        cp -f pyconfig.h ${prefix_dir}/include/
    fi 

    prefix_dir_lib=${prefix_dir}/lib/${var_target}/${USED_AndroidAPI}
    mkdir -p ${prefix_dir_lib}
    cp -fr ${prefix_tmp_dir}/lib/* ${prefix_dir_lib}/

    ln -s libpython${var_python_var_float}.so ${prefix_dir_lib}/libpython${var_python_ver}.so 

    prefix_dir_bin=${prefix_dir}/bin/${var_target}/${USED_AndroidAPI}
    mkdir -p ${prefix_dir_bin}
    cp -fr ${prefix_tmp_dir}/bin/* ${prefix_dir_bin}/

    make clean
    rm -fr ${prefix_tmp_dir}
done
