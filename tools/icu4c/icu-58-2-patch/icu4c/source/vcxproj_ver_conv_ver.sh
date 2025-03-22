#!/bin/sh

var_old=$1
var_new=$2


find ./ -type f -name '*.vcxproj' -exec sed -i 's/'${var_old}'/'${var_new}'/g' {} \;
