#!/bin/sh

AndoridNDKRoot=/d/android_devel/Android/Sdk/ndk/28.0.12674087
AndroidToolChain=${AndoridNDKRoot}/toolchains/llvm/prebuilt/windows-x86_64
AndroidAPI=21

AndroidTarget_list="
    armv7a-linux-androideabi
    i686-linux-android
    "

prefix_root_dir=stage32

rm -fr ${prefix_root_dir}

prefix_dir=${prefix_root_dir}/luajit21

prefix_dir_include=${prefix_dir}/include
mkdir -p ${prefix_dir_include}

cp -f *.h ${prefix_dir_include}/


for var_target in ${AndroidTarget_list} 
do
    echo "make" ${var_target}

    sh luajit_make_android_yggr.sh ${AndoridNDKRoot} ${AndroidToolChain} ${var_target} ${AndroidAPI} ${prefix_dir} -m32

done



