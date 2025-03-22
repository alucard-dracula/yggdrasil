#!/bin/sh

var_cross_build_dir=$1

var_local_dir=$(cd `dirname $0`; pwd)

mkdir -p "${var_local_dir}/data/out/build/icudt58l/curr"
mkdir -p "${var_local_dir}/data/out/build/icudt58l/lang"
mkdir -p "${var_local_dir}/data/out/build/icudt58l/region"
mkdir -p "${var_local_dir}/data/out/build/icudt58l/zone"
mkdir -p "${var_local_dir}/data/out/build/icudt58l/unit"
mkdir -p "${var_local_dir}/data/out/build/icudt58l/coll"
mkdir -p "${var_local_dir}/data/out/build/icudt58l/rbnf"
mkdir -p "${var_local_dir}/data/out/build/icudt58l/translit"
mkdir -p "${var_local_dir}/data/out/build/icudt58l/brkitr"

mkdir -p "${var_local_dir}/data/out/tmp/curr"
mkdir -p "${var_local_dir}/data/out/tmp/curr"
mkdir -p "${var_local_dir}/data/out/tmp/lang"
mkdir -p "${var_local_dir}/data/out/tmp/region"
mkdir -p "${var_local_dir}/data/out/tmp/zone"
mkdir -p "${var_local_dir}/data/out/tmp/unit"
mkdir -p "${var_local_dir}/data/out/tmp/coll"
mkdir -p "${var_local_dir}/data/out/tmp/rbnf"
mkdir -p "${var_local_dir}/data/out/tmp/translit"
mkdir -p "${var_local_dir}/data/out/tmp/brkitr"

AndroidNDK=/d/android_devel/Android/Sdk/ndk/28.0.12674087
AndroidAPI=21
AndroidToolChainPlatfrom=windows-x86_64

NDK_CFLAGS="\
    -fPIC \
    "

NDK_CXXFLAGS="\
    -std=c++11 \
    "

AndroidTarget_list="
    aarch64-linux-android
    armv7a-linux-androideabi
    x86_64-linux-android
    i686-linux-android
    riscv64-linux-android
    "

prefix_root_dir=${var_local_dir}/stage
prefix_tmp_dir=${prefix_root_dir}/build_tmp
prefix_dir=${prefix_root_dir}/libicu
prefix_dir_include=${prefix_dir}/include
prefix_dir_share=${prefix_dir}/share

rm -fr ${prefix_root_dir}
mkdir -p ${prefix_dir}

for var_target in ${AndroidTarget_list} 
do
    echo "make" ${var_target}
    rm -fr ${prefix_tmp_dir}

    if [ "${var_target}" = "riscv64-linux-android" ]; then
        if [ ${AndroidAPI} -ge 35 ]; then
            USED_AndroidAPI=${AndroidAPI}
        else
            USED_AndroidAPI=35
        fi
    else
        USED_AndroidAPI=${AndroidAPI}
    fi 

    sh build-libicu-android-cross_tpl.sh \
        ${prefix_tmp_dir} ${AndroidNDK} ${var_target} \
        ${USED_AndroidAPI} ${AndroidToolChainPlatfrom} \
        "${NDK_CFLAGS}" "${NDK_CXXFLAGS}" "${var_cross_build_dir}"

    if [ ! -d ${prefix_dir_include} ]; then
        cp -fr ${prefix_tmp_dir}/include ${prefix_dir}/
    fi 

    prefix_dir_bin=${prefix_dir}/bin/${var_target}/${USED_AndroidAPI}
    prefix_dir_sbin=${prefix_dir}/sbin/${var_target}/${USED_AndroidAPI}
    prefix_dir_lib=${prefix_dir}/lib/${var_target}/${USED_AndroidAPI}

    mkdir -p ${prefix_dir_bin}
    mkdir -p ${prefix_dir_sbin}
    mkdir -p ${prefix_dir_lib}

    cp -fr ${prefix_tmp_dir}/bin/* ${prefix_dir_bin}/
    cp -fr ${prefix_tmp_dir}/sbin/* ${prefix_dir_sbin}/
    cp -fr ${prefix_tmp_dir}/lib/* ${prefix_dir_lib}/

    if [ ! -d ${prefix_dir_share} ]; then
        cp -fr ${prefix_tmp_dir}/share ${prefix_dir}/
    fi 

    make clean
done
