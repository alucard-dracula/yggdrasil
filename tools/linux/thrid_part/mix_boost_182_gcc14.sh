#!/bin/sh

out_dir=boost_182
x64_dir=boost_182_x64
x32_dir=boost_182_x32

if [ -d ${out_dir} ];then
	rm -fr ${out_dir}
fi

cp -fr ${x64_dir} ${out_dir}

mv -f ${out_dir}/lib ${out_dir}/lib64

cp -fr ${x32_dir}/lib ${out_dir}/

rm -fr ${x64_dir}/*
rm -fr ${x32_dir}/*

ln -s ../${out_dir}/lib64 ${x64_dir}/lib
ln -s ../${out_dir}/include ${x64_dir}/include


ln -s ../${out_dir}/lib ${x32_dir}/lib
ln -s ../${out_dir}/include ${x32_dir}/include

ln -s ${out_dir} boost
