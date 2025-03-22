#!/bin/sh

var_prefix_dir=$1
AndroidNDK=$2
AndroidTarge=$3
AndroidAPI=$4
AndroidToolChainPlatfrom=$5

AndroidToolChain=${AndroidNDK}/toolchains/llvm/prebuilt/${AndroidToolChainPlatfrom}
AR=${AndroidToolChain}/bin/llvm-ar
CC=${AndroidToolChain}/bin/${AndroidTarge}${AndroidAPI}-clang
AS=$CC
CXX=${AndroidToolChain}/bin/${AndroidTarge}${AndroidAPI}-clang++
LD=${AndroidToolChain}/bin/ld
RANLIB=${AndroidToolChain}/bin/llvm-ranlib
STRIP=${AndroidToolChain}/bin/llvm-strip

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

./configure \
    --disable-tests \
    --enable-coredump=no \
    --host=x86_64-w64-mingw32 \
    --target=${AndroidTarge} \
    --build=$(echo ${AndroidTarge} | cut -d \- -f 1) \
    --prefix=${var_prefix_dir}
    
make -j12
make install