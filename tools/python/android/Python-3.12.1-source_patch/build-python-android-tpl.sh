#!/bin/sh

var_prefix_dir=$1
AndroidNDK=$2
AndroidTarge=$3
AndroidAPI=$4
AndroidToolChainPlatfrom=$5
NDK_CFLAGS=$6
NDK_LDFLAGS=$7
PythonToolChain=$8

AndroidToolChain=${AndroidNDK}/toolchains/llvm/prebuilt/${AndroidToolChainPlatfrom}
AR=${AndroidToolChain}/bin/llvm-ar
CC=${AndroidToolChain}/bin/${AndroidTarge}${AndroidAPI}-clang
AS=${CC}
CXX=${AndroidToolChain}/bin/${AndroidTarge}${AndroidAPI}-clang++
LD=${AndroidToolChain}/bin/ld
RANLIB=${AndroidToolChain}/bin/llvm-ranlib
STRIP=${AndroidToolChain}/bin/llvm-strip
CFLAGS=${NDK_CFLAGS}
LDFLAGS=${NDK_LDFLAGS}

ABIFLAGS=$(echo ${AndroidTarge} | cut -d \- -f 2)

export AndroidNDK
export AndroidTarge
export AndroidAPI
export AndroidToolChain
export AR
export CC
export AS
export CXX
export LD
export RANLIB
export STRIP
export CFLAGS
export LDFLAGS
export ABIFLAGS


./configure \
    --host=${AndroidTarge} \
    --build=$(echo ${AndroidTarge} | cut -d \- -f 1) \
    --enable-ipv6=no \
    --enable-shared=no \
    --disable-test-modules \
    --with-build-python="${PythonToolChain}" \
    ac_cv_file__dev_ptmx=no \
    ac_cv_file__dev_ptc=no \
    --prefix=${var_prefix_dir}

cp -f ./android_pyconfig_patch/pyconfig.h ./

make -j12
make install