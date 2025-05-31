#!/bin/bash

var_prefix_dir=$1
var_build_dir=$2
var_local_dir=$3

rm -fr "${var_prefix_dir}"

#include
mkdir -p "${var_prefix_dir}/include/sasl"
find "${var_local_dir}/include" -type f -name '*.h' -exec cp -f {} "${var_prefix_dir}/include/sasl/" \;

#lib
mkdir -p "${var_prefix_dir}/lib/sasl2"

find "${var_build_dir}/common/.libs" -type f -name '*.a' -exec cp -f {} "${var_prefix_dir}/lib/" \;
find "${var_build_dir}/sasldb/.libs" -type f -name '*.a' -exec cp -f {} "${var_prefix_dir}/lib/" \;

find "${var_build_dir}/lib/.libs" -name '*.dylib' -exec cp -f {} "${var_prefix_dir}/lib/" \;
find "${var_build_dir}/lib/.libs" -name '*.la*' -exec cp -f {} "${var_prefix_dir}/lib/" \;

find "${var_prefix_dir}/lib" -type f -name '*.dylib' -exec chmod a+x {} \;

find "${var_build_dir}/plugins/.libs" -name '*.so' -exec cp -f {} "${var_prefix_dir}/lib/sasl2/" \;
find "${var_build_dir}/plugins/.libs" -name '*.la*' -exec cp -f {} "${var_prefix_dir}/lib/sasl2/" \;

find "${var_prefix_dir}/lib/sasl2" -type f -name '*.so' -exec chmod a+x {} \;

#bin
mkdir -p "${var_prefix_dir}/sbin"

cp -f "${var_build_dir}/saslauthd/saslauthd" "${var_prefix_dir}/sbin/"
cp -f "${var_build_dir}/saslauthd/testsaslauthd" "${var_prefix_dir}/sbin/"
cp -f "${var_build_dir}/utils/pluginviewer" "${var_prefix_dir}/sbin/"

find "${var_prefix_dir}/bin" -type f -exec chmod a+x {} \;