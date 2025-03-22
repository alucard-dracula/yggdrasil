#!/bin/sh

var_local_dir=$(cd `dirname $0`; pwd)

AndroidNDK=/d/android_devel/Android/Sdk/ndk/28.0.12674087
AndroidAPI=21
AndroidToolChainPlatfrom=windows-x86_64

AndroidTarget_list="
    aarch64-linux-android
    armv7a-linux-androideabi
    x86_64-linux-android
    i686-linux-android
    riscv64-linux-android
    "
    
prefix_root_dir=${var_local_dir}/stage

prefix_tmp_dir=${prefix_root_dir}/build_tmp
prefix_dir=${prefix_root_dir}/libunwind

prefix_dir_include=${prefix_dir}/include

rm -fr ${prefix_root_dir}
mkdir -p ${prefix_dir}

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

    sh build-libunwind-android_tpl.sh ${prefix_tmp_dir} ${AndroidNDK} ${var_target} ${USED_AndroidAPI} ${AndroidToolChainPlatfrom}

    if [ ! -d ${prefix_dir_include} ]; then
        cp -fr ${prefix_tmp_dir}/include ${prefix_dir}/
    fi 

    prefix_dir_lib=${prefix_dir}/lib/${var_target}/${USED_AndroidAPI}
    mkdir -p ${prefix_dir_lib}
    cp -fr ${prefix_tmp_dir}/lib/* ${prefix_dir_lib}/
    make clean
    rm -fr ${prefix_tmp_dir}
done
