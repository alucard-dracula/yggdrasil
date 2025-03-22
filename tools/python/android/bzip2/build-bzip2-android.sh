#!/bin/sh

AndoridNDKRoot=/d/android_devel/Android/Sdk/ndk/28.0.12674087
AndroidToolChain=${AndoridNDKRoot}/toolchains/llvm/prebuilt/windows-x86_64
AndroidAPI=21

#target_name see AndroidNDKRoot/toolchains/llvm/prebuilt/windows-x86_64/bin

AndroidTarget_list="
    aarch64-linux-android
    armv7a-linux-androideabi
    x86_64-linux-android
    i686-linux-android
    riscv64-linux-android
    "
    
export AndroidToolChain
export AndroidAPI

prefix_root_dir=stage

rm -fr ${prefix_root_dir}

prefix_dir=${prefix_root_dir}/bzip2

prefix_dir_include=${prefix_dir}/include
mkdir -p ${prefix_dir_include}
cp -f bzlib.h ${prefix_dir_include}

for var_target in ${AndroidTarget_list} 
do
    echo "make" ${var_target}
    AndroidTarget=${var_target}

    if [ "${var_target}" = "riscv64-linux-android" ]; then
        if [ ${AndroidAPI} -ge 35 ]; then
            USED_AndroidAPI=${AndroidAPI}
        else
            USED_AndroidAPI=35
        fi
    else
        USED_AndroidAPI=${AndroidAPI}
    fi 

    export USED_AndroidAPI

    prefix_dir_lib=${prefix_dir}/lib/${AndroidTarget}/${USED_AndroidAPI}
    mkdir -p ${prefix_dir_lib}

    export AndroidTarget

    make -f Makefile-android clean
    make -f Makefile-android libbz2.a
    cp -f libbz2.a ${prefix_dir_lib}
done



