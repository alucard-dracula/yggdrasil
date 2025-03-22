#!/bin/sh

var_prefix_dir=$1
AndroidNDK=$2
AndroidTarge=$3
AndroidAPI=$4
AndroidToolChainPlatfrom=$5
NDK_CFLAGS=$6
NDK_CXXFLAGS=$7
cross_build_dir=$8

AndroidToolChain=${AndroidNDK}/toolchains/llvm/prebuilt/${AndroidToolChainPlatfrom}
AR=${AndroidToolChain}/bin/llvm-ar
CC=${AndroidToolChain}/bin/${AndroidTarge}${AndroidAPI}-clang
AS=$CC
CXX=${AndroidToolChain}/bin/${AndroidTarge}${AndroidAPI}-clang++
LD=${AndroidToolChain}/bin/ld
RANLIB=${AndroidToolChain}/bin/llvm-ranlib
STRIP=${AndroidToolChain}/bin/llvm-strip
CFLAGS=${NDK_CFLAGS}
CXXFLAGS=${NDK_CXXFLAGS}

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
export CXXFLAGS

../../configure \
    --enable-static=yes \
    --enable-tests=no \
    --enable-samples=no \
    --enable-renaming=yes \
    --enable-tracing=yes \
    --enable-rpath=yes \
    --with-data-packaging=archive \
    --with-cross-build=${cross_build_dir} \
    --host=${AndroidTarge} \
    --build=$(echo ${AndroidTarge} | cut -d \- -f 1) \
    --prefix=${var_prefix_dir}

make -j12
make install