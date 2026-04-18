#!/bin/bash

AndoridNDKRoot=$1
AndroidToolChain=$2
AndroidTarget=$3
AndroidAPI=$4
perfix_dir=$5
host_bis=$6

rm *.so
rm *.a
rm *.o
rm host/*.o

luajit_host_cc=gcc
luajit_host_cflags="${host_bis} -O3"
luajit_host_ldflags="${host_bis} -s"

luajit_cross="${AndroidToolChain}/bin/${AndroidTarget}${AndroidAPI}-"
luajit_static_cc="${luajit_cross}clang"
luajit_dynamic_cc="${luajit_cross}clang"
luajit_target_ld=${luajit_static_cc}
luajit_target_ar="${AndroidToolChain}/bin/llvm-ar rcus"
luajit_target_strip=${AndroidToolChain}/bin/llvm-strip
luajit_target_sys=Linux

luajit_target_cflag_debug="-fPIC -O0 -g -DLUA_USE_LINUX"
luajit_target_ldflag_debug=

luajit_target_cflag_release="-fPIC -O3 -DLUA_USE_LINUX"
luajit_target_ldflag_release="-s"

if [ "${AndroidTarget}" = "aarch64-linux-android" ]; then
    perfix_bin_dir=$perfix_dir/bin/arm64-v8a
    perfix_bin_debug_dir=$perfix_dir/bin-debug/arm64-v8a
elif [ "${AndroidTarget}" = "armv7a-linux-androideabi" ]; then
    perfix_bin_dir=$perfix_dir/bin/armeabi-v7a
    perfix_bin_debug_dir=$perfix_dir/bin-debug/armeabi-v7a
elif [ "${AndroidTarget}" = "x86_64-linux-android" ]; then
    perfix_bin_dir=$perfix_dir/bin/x86_64
    perfix_bin_debug_dir=$perfix_dir/bin-debug/x86_64
elif [ "${AndroidTarget}" = "i686-linux-android" ]; then
    perfix_bin_dir=$perfix_dir/bin/x86
    perfix_bin_debug_dir=$perfix_dir/bin-debug/x86
else
    perfix_bin_dir=$perfix_dir/bin/${AndroidTarget}
    perfix_bin_debug_dir=$perfix_dir/bin-debug/${AndroidTarget}
fi

perfix_lib_dir=$perfix_dir/lib/${AndroidTarget}/${AndroidAPI}

mkdir -p ${perfix_bin_dir}
mkdir -p ${perfix_bin_debug_dir}
mkdir -p ${perfix_lib_dir}

make clean

make -j12 -f Makefile_yggr \
    NODOTABIVER=jit501-clang-d \
    HOST_CC="${luajit_host_cc}" \
    HOST_CFLAGS="${luajit_host_cflags}" \
    HOST_LDFLAGS="${luajit_host_ldflags}" \
    CROSS="${luajit_cross}" \
    STATIC_CC="${luajit_static_cc}" \
    DYNAMIC_CC="${luajit_dynamic_cc}" \
    TARGET_LD="${luajit_target_ld}" \
    TARGET_AR="${luajit_target_ar}" \
    TARGET_STRIP="${luajit_target_strip}" \
    TARGET_SYS="${luajit_target_sys}" \
    TARGET_CFLAGS="${luajit_target_cflag_debug}" \
    TARGET_LDFLAGS="${luajit_target_ldflag_debug}" 

cp -f libluajit501-clang-d.so "${perfix_lib_dir}/libluajit501_ts-clang-d.so"
mv -f libluajit501-clang-d.so "${perfix_lib_dir}/"
mv -f libluajit501-clang-d.a "${perfix_lib_dir}/libluajit501_ts-clang-d.a"
mv -f libluajit501-clang-d.a "${perfix_lib_dir}/"

make clean

rm *.o
rm host/*.o

make -j12 -f Makefile_yggr \
    NODOTABIVER=jit501_static-clang-d \
    HOST_CC="${luajit_host_cc}" \
    HOST_CFLAGS="${luajit_host_cflags}" \
    HOST_LDFLAGS="${luajit_host_ldflags}" \
    CROSS="${luajit_cross}" \
    STATIC_CC="${luajit_static_cc}" \
    DYNAMIC_CC="${luajit_dynamic_cc}" \
    TARGET_LD="${luajit_target_ld}" \
    TARGET_AR="${luajit_target_ar}" \
    TARGET_STRIP="${luajit_target_strip}" \
    TARGET_SYS="${luajit_target_sys}" \
    TARGET_CFLAGS="${luajit_target_cflag_debug}" \
    TARGET_LDFLAGS="${luajit_target_ldflag_debug}" \
    BUILDMODE=static

cp -f libluajit501_static-clang-d.a "${perfix_lib_dir}/libluajit501_ts_static-clang-d.a"
mv -f libluajit501_static-clang-d.a "${perfix_lib_dir}/"
cp -f luajit "${perfix_bin_debug_dir}/lua"
cp -f luajit "${perfix_bin_debug_dir}/luac"
mv -f luajit "${perfix_bin_debug_dir}/"

make clean

rm *.o
rm host/*.o

make -j12 -f Makefile_yggr \
    NODOTABIVER=jit501-clang \
    HOST_CC="${luajit_host_cc}" \
    HOST_CFLAGS="${luajit_host_cflags}" \
    HOST_LDFLAGS="${luajit_host_ldflags}" \
    CROSS="${luajit_cross}" \
    STATIC_CC="${luajit_static_cc}" \
    DYNAMIC_CC="${luajit_dynamic_cc}" \
    TARGET_LD="${luajit_target_ld}" \
    TARGET_AR="${luajit_target_ar}" \
    TARGET_STRIP="${luajit_target_strip}" \
    TARGET_SYS="${luajit_target_sys}" \
    TARGET_CFLAGS="${luajit_target_cflag_release}" \
    TARGET_LDFLAGS="${luajit_target_ldflag_release}" 

cp -f libluajit501-clang.so "${perfix_lib_dir}/libluajit501_ts-clang.so"
mv -f libluajit501-clang.so "${perfix_lib_dir}/"
cp -f libluajit501-clang.a "${perfix_lib_dir}/libluajit501_ts-clang.a"
mv -f libluajit501-clang.a "${perfix_lib_dir}/"

make clean

rm *.o
rm host/*.o

make -j12 -f Makefile_yggr \
    NODOTABIVER=jit501_static-clang \
    HOST_CC="${luajit_host_cc}" \
    HOST_CFLAGS="${luajit_host_cflags}" \
    HOST_LDFLAGS="${luajit_host_ldflags}" \
    CROSS="${luajit_cross}" \
    STATIC_CC="${luajit_static_cc}" \
    DYNAMIC_CC="${luajit_dynamic_cc}" \
    TARGET_LD="${luajit_target_ld}" \
    TARGET_AR="${luajit_target_ar}" \
    TARGET_STRIP="${luajit_target_strip}" \
    TARGET_SYS="${luajit_target_sys}" \
    TARGET_CFLAGS="${luajit_target_cflag_release}" \
    TARGET_LDFLAGS="${luajit_target_ldflag_release}" \
    BUILDMODE=static

cp -f libluajit501_static-clang.a "${perfix_lib_dir}/libluajit501_ts_static-clang.a"
mv -f libluajit501_static-clang.a "${perfix_lib_dir}/"
cp -f luajit "${perfix_bin_dir}/lua"
cp -f luajit "${perfix_bin_dir}/luac"
mv -f luajit "${perfix_bin_dir}/"

make clean
rm *.o
rm host/*.o
