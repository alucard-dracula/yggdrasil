#!/bin/sh

var_old=$1
var_new=$2


find ./ -type f -name '*.vcxproj' -exec sed -i 's/'${var_old}'-/'${var_new}'/g' {} \;
find ./ -type f -name '*.vcxproj' -exec sed -i 's/'${var_old}/'${var_new}'/g' {} \;

sed -i 's/VC_VER=\"-vc\$(PlatformToolsetVersion)\".*CFG=/VC_VER=\"-vc\$(PlatformToolsetVersion)\" CFG=/g' ./data/makedata.vcxproj
sed -i 'ICU_LIB_TARGET=\$(DLL_OUTPUT)\\\$(U_ICUDATA_NAME)\$(VC_VER)\.dll/ICU_LIB_TARGET=\$(DLL_OUTPUT)\\\$(U_ICUDATA_NAME)\.dll/g' ./data/makedata.mak
