#!/bin/bash

var_dir=$(cd `dirname $0`; pwd)

var_stage_dir="${var_dir}/stage_prefix"

var_stage_dir_macos="${var_dir}/stage_prefix/lmdb-macosx/lib"
ln -s "liblmdb-clang-darwin16.a" "${var_stage_dir_macos}/liblmdb.a" 

var_stage_dir_macos="${var_dir}/stage_prefix/lmdb-iphoneos/lib"
ln -s "liblmdb-clang-darwin16.a" "${var_stage_dir_macos}/liblmdb.a" 

var_stage_dir_macos="${var_dir}/stage_prefix/lmdb-iphonesimulator/lib"
ln -s "liblmdb-clang-darwin16.a" "${var_stage_dir_macos}/liblmdb.a" 
