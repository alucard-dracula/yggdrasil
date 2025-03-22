#!/bin/sh

var_dir=$(cd `dirname $0`; pwd)

mkdir -p "${var_dir}/data/out/build/icudt58l/curr"
mkdir -p "${var_dir}/data/out/build/icudt58l/lang"
mkdir -p "${var_dir}/data/out/build/icudt58l/region"
mkdir -p "${var_dir}/data/out/build/icudt58l/zone"
mkdir -p "${var_dir}/data/out/build/icudt58l/unit"
mkdir -p "${var_dir}/data/out/build/icudt58l/coll"
mkdir -p "${var_dir}/data/out/build/icudt58l/rbnf"
mkdir -p "${var_dir}/data/out/build/icudt58l/translit"
mkdir -p "${var_dir}/data/out/build/icudt58l/brkitr"

mkdir -p "${var_dir}/data/out/tmp/curr"
mkdir -p "${var_dir}/data/out/tmp/lang"
mkdir -p "${var_dir}/data/out/tmp/region"
mkdir -p "${var_dir}/data/out/tmp/zone"
mkdir -p "${var_dir}/data/out/tmp/unit"
mkdir -p "${var_dir}/data/out/tmp/coll"
mkdir -p "${var_dir}/data/out/tmp/rbnf"
mkdir -p "${var_dir}/data/out/tmp/translit"
mkdir -p "${var_dir}/data/out/tmp/brkitr"


./icu4c_build_mingw_tpl.sh "${var_dir}/libicu_mgw14_x64"
