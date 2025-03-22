#!/bin/sh

var_local_dir=$(cd `dirname $0`; pwd)

AndroidNDK=/d/android_devel/Android/Sdk/ndk/28.0.12674087
AndroidThridPartLocal=${AndroidNDK}/../../../../ndk_third_part_local

AndroidAPI=21
AndroidToolChainPlatfrom=windows-x86_64

NDK_CFLAGS="\
    -fPIC \
    -Dasm=__asm__ \
    -I${AndroidThridPartLocal}/bzip2/include \
    -I${AndroidThridPartLocal}/liblzma_xz/include \
    -I${AndroidThridPartLocal}/readline/include \
    -I${AndroidThridPartLocal}/libffi/include \
    -I${AndroidThridPartLocal}/libunwind/include \
    "

PythonToolChain=/d/Python312/python

 AndroidTarget_list="
    aarch64-linux-android
    armv7a-linux-androideabi
    x86_64-linux-android
    i686-linux-android
    riscv64-linux-android
    "

prefix_root_dir=${var_local_dir}/../python_stage

prefix_tmp_dir=${prefix_root_dir}/build_tmp
prefix_dir=${prefix_root_dir}/python3.12.1

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
        -L${AndroidThridPartLocal}/bzip2/lib/${var_target}/${USED_AndroidAPI} \
        -L${AndroidThridPartLocal}/liblzma_xz/lib/${var_target}/${USED_AndroidAPI} \
        -L${AndroidThridPartLocal}/readline/lib/${var_target}/${USED_AndroidAPI} \
        -L${AndroidThridPartLocal}/libffi/lib/${var_target}/${USED_AndroidAPI} \
        -L${AndroidThridPartLocal}/libunwind/lib/${var_target}/${USED_AndroidAPI} \
        "

    make clean
    sh build-python-android-tpl.sh \
        "${prefix_tmp_dir}" "${AndroidNDK}" "${var_target}" "${USED_AndroidAPI}" \
        "${AndroidToolChainPlatfrom}" "${NDK_CFLAGS}" "${NDK_LDFLAGS}" "${PythonToolChain}"

#    cp -fr ${prefix_tmp_dir} "${prefix_tmp_dir}-${var_target}"
#    cp -f pyconfig.h "${prefix_tmp_dir}-${var_target}/"
    
    if [ ! -d ${prefix_dir_include} ]; then
        cp -fr ${prefix_tmp_dir}/include/python3.12 ${prefix_dir}/include
        cp -f pyconfig.h ${prefix_dir}/include/
    fi 

    prefix_dir_lib=${prefix_dir}/lib/${var_target}/${USED_AndroidAPI}
    mkdir -p ${prefix_dir_lib}
    cp -fr ${prefix_tmp_dir}/lib/* ${prefix_dir_lib}/
    ln -s libpython3.12.a ${prefix_dir_lib}/libpython312.a
    make clean
    rm -fr ${prefix_tmp_dir}
done
