#!/bin/sh

var_src_root_dir_arm64=$1
var_src_root_dir_x86_64=$2
var_dst_root_dir=$3
var_dst_root_sample=$4

echo "var_src_root_dir_arm64 = ${var_src_root_dir_arm64}"
echo "var_src_root_dir_x86_64 = ${var_src_root_dir_x86_64}"
echo "var_dst_root_dir = ${var_dst_root_dir}"
echo "var_dst_root_sample = ${var_dst_root_sample}"


if [ "${var_dst_root_sample}" = "x86_64" ]; then
    var_src_root_dir=${var_src_root_dir_x86_64}
    var_att_root_dir=${var_src_root_dir_arm64}
    var_src="x86_64"
    var_att="arm64"
else
    var_src_root_dir=${var_src_root_dir_arm64}
    var_att_root_dir=${var_src_root_dir_x86_64}
    var_src="arm64"
    var_att="x86_64"
fi


if [ "${var_src_root_dir}" = "" ]; then
    var_src_root_dir=./not_exists
fi

if [ "${var_att_root_dir}" = "" ]; then
    var_att_root_dir=./not_exists
fi 

if [ "${var_dst_root_dir}" = "" ]; then
    var_dst_root_dir=./not_exists
fi 

rm -fr "${var_dst_root_dir}"
cp -fr "${var_src_root_dir}" "${var_dst_root_dir}"

if [ -d "${var_dst_root_dir}/bin" ]; then
    cp -fr "${var_dst_root_dir}/bin" "${var_dst_root_dir}/bin-${var_src}"
    find "${var_dst_root_dir}/bin" -type f -perm +555 -exec rm -f {} \;
fi

if [ -d "${var_dst_root_dir}/lib" ]; then
    cp -fr "${var_dst_root_dir}/lib" "${var_dst_root_dir}/lib-${var_src}"
    find "${var_dst_root_dir}/lib" -type f -name "*.a" -exec rm -f {} \;
    find "${var_dst_root_dir}/lib" -type f -name "*.dylib" -exec rm -f {} \;
    find "${var_dst_root_dir}/lib" -type f -name "*.so" -exec rm -f {} \;
fi

if [ -d "${var_dst_root_dir}/include" ]; then
    mv -f "${var_dst_root_dir}/include" "${var_dst_root_dir}/include-${var_src}"
    ln -s "include-${var_src}" "${var_dst_root_dir}/include"
fi

if [ -d "${var_att_root_dir}/bin" ]; then
    cp -fr "${var_att_root_dir}/bin" "${var_dst_root_dir}/bin-${var_att}"
fi

if [ -d "${var_att_root_dir}/lib" ]; then
    cp -fr "${var_att_root_dir}/lib" "${var_dst_root_dir}/lib-${var_att}"
fi

if [ -d "${var_att_root_dir}/include" ]; then
    cp -fr "${var_att_root_dir}/include" "${var_dst_root_dir}/include-${var_att}"
fi

#.a
var_lipo_dir_src=${var_dst_root_dir}/lib-${var_src}
var_lipo_dir_att=${var_dst_root_dir}/lib-${var_att}
var_lipo_dir_fat=${var_dst_root_dir}/lib

mkdir -p "${var_lipo_dir_fat}"

var_src_file_list=$(find ${var_lipo_dir_src} -type f -name '*.a')

for var_fpath in ${var_src_file_list} 
do
    var_att_fpath=${var_fpath/lib-${var_src}/lib-${var_att}}
    var_fat_fpath=${var_fpath/lib-${var_src}/lib}
    var_cmd_create="lipo -create ${var_fpath} ${var_att_fpath} -output ${var_fat_fpath}"
    var_cmd_check="lipo -info ${var_fat_fpath}"
    echo "${var_cmd_create}"
    eval ${var_cmd_create}
    eval ${var_cmd_check}
done

#.dylib
var_lipo_dir_src=${var_dst_root_dir}/lib-${var_src}
var_lipo_dir_att=${var_dst_root_dir}/lib-${var_att}
var_lipo_dir_fat=${var_dst_root_dir}/lib

mkdir -p "${var_lipo_dir_fat}"

var_src_file_list=$(find ${var_lipo_dir_src} -type f -name '*.dylib')

for var_fpath in ${var_src_file_list} 
do
    var_att_fpath=${var_fpath/lib-${var_src}/lib-${var_att}}
    var_fat_fpath=${var_fpath/lib-${var_src}/lib}
    var_cmd_create="lipo -create ${var_fpath} ${var_att_fpath} -output ${var_fat_fpath}"
    var_cmd_check="lipo -info ${var_fat_fpath}"
    echo "${var_cmd_create}"
    eval ${var_cmd_create}
    eval ${var_cmd_check}
done

#.so
var_lipo_dir_src=${var_dst_root_dir}/lib-${var_src}
var_lipo_dir_att=${var_dst_root_dir}/lib-${var_att}
var_lipo_dir_fat=${var_dst_root_dir}/lib

mkdir -p "${var_lipo_dir_fat}"

var_src_file_list=$(find ${var_lipo_dir_src} -type f -name '*.so')

for var_fpath in ${var_src_file_list} 
do
    var_att_fpath=${var_fpath/lib-${var_src}/lib-${var_att}}
    var_fat_fpath=${var_fpath/lib-${var_src}/lib}
    var_cmd_create="lipo -create ${var_fpath} ${var_att_fpath} -output ${var_fat_fpath}"
    var_cmd_check="lipo -info ${var_fat_fpath}"
    echo "${var_cmd_create}"
    eval ${var_cmd_create}
    eval ${var_cmd_check}
done

# exe
var_lipo_dir_src=${var_dst_root_dir}/bin-${var_src}
var_lipo_dir_att=${var_dst_root_dir}/bin-${var_att}
var_lipo_dir_fat=${var_dst_root_dir}/bin

mkdir -p "${var_lipo_dir_fat}"

var_src_file_list=$(find ${var_lipo_dir_src} -type f -perm +555)

for var_fpath in ${var_src_file_list} 
do
    var_att_fpath=${var_fpath/bin-${var_src}/bin-${var_att}}
    var_fat_fpath=${var_fpath/bin-${var_src}/bin}
    if [ -f "${var_att_fpath}" ];then

        var_file_info_src="$(file ${var_fpath})"
        var_file_info_attr="$(file ${var_att_fpath})"
        if [ "$(echo ${var_file_info_src##*:} | grep ${var_src})" != "" ] \
            && [ "$(echo ${var_file_info_attr##*:} | grep ${var_att})" != "" ]; then

            var_cmd_create="lipo -create ${var_fpath} ${var_att_fpath} -output ${var_fat_fpath}"
            var_cmd_check="lipo -info ${var_fat_fpath}"
            echo "${var_cmd_create}"
            eval ${var_cmd_create}
            eval ${var_cmd_check}
        else
             cp -f "${var_fpath}" "${var_fat_fpath}"
        fi
    else
        cp -f "${var_fpath}" "${var_fat_fpath}"
    fi
done
