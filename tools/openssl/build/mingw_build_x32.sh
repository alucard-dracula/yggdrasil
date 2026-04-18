#!/bin/bash

var_dir=$(cd `dirname $0`; pwd)


gcc_mark="mgw15-x32"

dir_debug="${gcc_mark}-debug"
dir_debug_static="${gcc_mark}-static-debug"

dir_release="${gcc_mark}-release"
dir_release_static="${gcc_mark}-static-release"

./mingw_build_tpl.sh "debug-mingw" "${var_dir}/stage/${dir_debug}" "${var_dir}/stage/${dir_debug}/ssl" "shared"
./mingw_build_tpl.sh "debug-mingw" "${var_dir}/stage/${dir_debug_static}" "${var_dir}/stage/${dir_debug_static}/ssl" "no-shared"

./mingw_build_tpl.sh "mingw" "${var_dir}/stage/${dir_release}" "${var_dir}/stage/${dir_release}/ssl" "shared"
./mingw_build_tpl.sh "mingw" "${var_dir}/stage/${dir_release_static}" "${var_dir}/stage/${dir_release_static}/ssl" "no-shared"



