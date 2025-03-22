#!/bin/sh

var_prefix_dir=$1
var_openssl_dir=$2
AndroidNDK=$3
AndroidTarge=$4
AndroidAPI=$5
AndroidToolChainPlatfrom=$6

AndroidToolChain=${AndroidNDK}/toolchains/llvm/prebuilt/${AndroidToolChainPlatfrom}
AndroidToolChainBin=${AndroidToolChain}/bin

ANDROID_NDK_ROOT=${AndroidNDK}

if [ "${AndroidTarge}" = "aarch64-linux-android" ]; then
    ssl_target=android-arm64
fi

if [ "${AndroidTarge}" = "armv7a-linux-androideabi" ]; then
    ssl_target=android-arm
fi

if [ "${AndroidTarge}" = "riscv64-linux-android" ]; then
    if [ ! ${AndroidAPI} -ge 35 ]; then
        AndroidAPI=35
    fi
    ssl_target=android-riscv64
fi

if [ "${AndroidTarge}" = "x86_64-linux-android" ]; then
    ssl_target=android-x86_64
fi

if [ "${AndroidTarge}" = "i686-linux-android" ]; then
    ssl_target=android-x86
fi

export ANDROID_NDK_ROOT
export PATH=${AndroidToolChainBin}:$PATH

echo "${ssl_target}"

perl Configure \
        ${ssl_target} \
        -D__ANDROID_API__=${AndroidAPI} \
        --openssldir=${var_openssl_dir} \
        --prefix=${var_prefix_dir}


#make depend # openssl 3.4.0 not need the code
make -j12
make install