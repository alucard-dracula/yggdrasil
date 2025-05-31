#!/bin/bash

var_dir=$(cd `dirname $0`; pwd)

var_outdir="${var_dir}/stage"

rm -fr "${var_outdir}"

mkdir -p "${var_outdir}/include"

cp -fr lib64 "${var_outdir}/"
cp -fr lib "${var_outdir}/"

#fix lib64

if [ -f "${var_outdir}/lib64/liblmdb-vc100.lib" ]; then
    cp -f "${var_outdir}/lib64/liblmdb-vc100.lib" "${var_outdir}/lib64/liblmdb.lib"
else
    if [ -f "${var_outdir}/lib64/liblmdb-vc143.lib" ]; then
         cp -f "${var_outdir}/lib64/liblmdb-vc143.lib" "${var_outdir}/lib64/liblmdb.lib"
    fi
fi

if [ -f "${var_outdir}/lib/liblmdb-vc100.lib" ]; then
    cp -f "${var_outdir}/lib/liblmdb-vc100.lib" "${var_outdir}/lib/liblmdb.lib"
else
    if [ -f "${var_outdir}/lib/liblmdb-vc143.lib" ]; then
         cp -f "${var_outdir}/lib/liblmdb-vc143.lib" "${var_outdir}/lib/liblmdb.lib"
    fi
fi

if [ -f "${var_outdir}/lib64/liblmdb-mgw14.a" ]; then
    cp -f "${var_outdir}/lib64/liblmdb-mgw14.a" "${var_outdir}/lib64/liblmdb.a"
fi

if [ -f "${var_outdir}/lib/liblmdb-mgw14.a" ]; then
    cp -f "${var_outdir}/lib/liblmdb-mgw14.a" "${var_outdir}/lib/liblmdb.a"
fi

if [ -f "${var_outdir}/lib64/liblmdb-gcc14.a" ]; then
    ln -s "liblmdb-gcc14.a" "${var_outdir}/lib64/liblmdb.a"
fi

if [ -f "${var_outdir}/lib/liblmdb-gcc14.a" ]; then
    ln -s "liblmdb-gcc14.a" "${var_outdir}/lib/liblmdb.a"
fi

cp "${var_dir}/lmdb.h" "${var_outdir}/include/"
cp "${var_dir}/midl.h" "${var_outdir}/include/"
