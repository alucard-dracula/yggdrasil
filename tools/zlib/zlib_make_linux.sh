#!/bin/sh

#:In order to maintain the habit of using LINUX, zlib only compiles the RELEASE version

CFLAGS="-O3 -m64 -Wall"
LDFLAGS="-s -m64"

export CFLAGS
export LDFLAGS

out_dir_x64=libzlib_x64
out_dir_x32=libzlib_x32

out_dir_mix=libzlib1.3

rm -fr ${out_dir_x64}
rm -fr ${out_dir_x32}
rm -fr ${out_dir_mix}

sh configure --prefix="${out_dir_x64}"
make all
make install
make clean

#----32bit-----

CFLAGS="-O3 -m32 -Wall"
LDFLAGS="-s -m32"

export CFLAGS
export LDFLAGS

sh configure --prefix="${out_dir_x32}"
make all
make install
make clean

cp -fr ${out_dir_x64} ${out_dir_mix}
mv -f ${out_dir_mix}/lib ${out_dir_mix}/lib64
cp -fr ${out_dir_x32}/lib ${out_dir_mix}/

rm -fr ${out_dir_mix}/lib64/pkgconfig
rm -fr ${out_dir_mix}/lib/pkgconfig
