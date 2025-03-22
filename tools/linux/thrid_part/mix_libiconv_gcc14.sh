#!/bin/sh

out_dir=libiconv_117_gcc14
x64_dir=libiconv_117_x64
x32_dir=libiconv_117_x32

if [ -d ${out_dir} ];then
	rm -fr ${out_dir}
fi

cp -fr ${x64_dir} ${out_dir}

mv -f ${out_dir}/bin ${out_dir}/bin64
#mv -f ${out_dir}/sbin ${out_dir}/sbin64
mv -f ${out_dir}/lib ${out_dir}/lib64

cp -fr ${x32_dir}/bin ${out_dir}/
#cp -fr ${x32_dir}/sbin ${out_dir}/
cp -fr ${x32_dir}/lib ${out_dir}/

rm -fr ${x64_dir}
rm -fr ${x32_dir}

ln -s ${out_dir} libiconv
