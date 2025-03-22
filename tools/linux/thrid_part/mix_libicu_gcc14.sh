#!/bin/sh

out_dir=libicu58_gcc14
x64_dir=libicu58_gcc14_x64
x32_dir=libicu58_gcc14_x32

if [ -d ${out_dir} ];then
	rm -fr ${out_dir}
fi

cp -fr ${x64_dir} ${out_dir}

mv -f ${out_dir}/bin ${out_dir}/bin64
mv -f ${out_dir}/sbin ${out_dir}/sbin64
mv -f ${out_dir}/lib ${out_dir}/lib64

cp -fr ${x32_dir}/bin ${out_dir}/
cp -fr ${x32_dir}/sbin ${out_dir}/
cp -fr ${x32_dir}/lib ${out_dir}/

rm -fr ${x64_dir}/*
rm -fr ${x32_dir}/*

ln -s ../${out_dir}/bin64 ${x64_dir}/bin
ln -s ../${out_dir}/lib64 ${x64_dir}/lib
ln -s ../${out_dir}/sbin64 ${x64_dir}/sbin
ln -s ../${out_dir}/include ${x64_dir}/include
ln -s ../${out_dir}/share ${x64_dir}/share


ln -s ../${out_dir}/bin ${x32_dir}/bin
ln -s ../${out_dir}/lib ${x32_dir}/lib
ln -s ../${out_dir}/sbin ${x32_dir}/sbin
ln -s ../${out_dir}/include ${x32_dir}/include
ln -s ../${out_dir}/share ${x32_dir}/share

ln -s ${out_dir} libicu
