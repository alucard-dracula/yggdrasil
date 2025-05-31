#!/bin/bash

var_usr_cmake_targer=Xcode
var_darwin_sdk_name=iphonesimulator
var_darwin_sdk_ver_min=15.6
var_darwin_arch="x86_64"
var_usr_cmake_ld_arch_tag=x
var_usr_cmake_c_standard=99
var_usr_cmake_cxx_standard=20
var_clear_other_build_lib=yes
var_clear_other_build_bin=yes

sh cmake-build-tpl_call.sh \
	${var_usr_cmake_targer} \
	${var_darwin_sdk_name} \
	${var_darwin_sdk_ver_min} \
	"${var_darwin_arch}" \
	${var_usr_cmake_c_standard} \
	${var_usr_cmake_cxx_standard} \
	${var_usr_cmake_ld_arch_tag} \
	${var_clear_other_build_lib} \
	${var_clear_other_build_bin}

