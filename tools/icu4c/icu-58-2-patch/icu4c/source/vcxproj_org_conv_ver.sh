#!/bin/sh

var_old=$1
var_new=$2


find ./ -type f -name '*.vcxproj' -exec sed -i 's/'${var_old}'\.dll/'${var_new}'\.dll/g' {} \;
find ./ -type f -name '*.vcxproj' -exec sed -i 's/d'${var_new}'\.dll/'${var_new}'-d\.dll/g' {} \;

find ./ -type f -name '*.vcxproj' -exec sed -i 's/'${var_old}'\.lib/'${var_new}'\.lib/g' {} \;
find ./ -type f -name '*.vcxproj' -exec sed -i 's/d'${var_new}'\.lib/'${var_new}'-d\.lib/g' {} \;

find ./ -type f -name '*.vcxproj' -exec sed -i 's/winmm'${var_new}'\.lib/winmm\.lib/g' {} \;
find ./ -type f -name '*.vcxproj' -exec sed -i 's/odbc32'${var_new}'\.lib/odbc32\.lib/g' {} \;
find ./ -type f -name '*.vcxproj' -exec sed -i 's/odbccp32'${var_new}'\.lib/odbccp32\.lib/g' {} \;
find ./ -type f -name '*.vcxproj' -exec sed -i 's/uconvmsg'${var_new}'\.lib/uconvmsg\.lib/g' {} \;


find ./ -type f -name '*.vcxproj' -exec sed -i 's/'${var_old}'\.pdb/'${var_new}'\.pdb/g' {} \;
find ./ -type f -name '*.vcxproj' -exec sed -i 's/d'${var_new}'\.pdb/'${var_new}'-d\.pdb/g' {} \;

find ./ -type f -name '*.vcxproj' -exec sed -i 's/'${var_old}'\.tlb/'${var_new}'\.tlb/g' {} \;
find ./ -type f -name '*.vcxproj' -exec sed -i 's/d'${var_new}'\.tlb/'${var_new}'-d\.tlb/g' {} \;

sed -i 's/VC_VER=\"-vc\$(PlatformToolsetVersion)\".*CFG=/VC_VER=\"-vc\$(PlatformToolsetVersion)\" CFG=/g' ./data/makedata.vcxproj
sed -i 's/NMAKE \/f makedata\.mak ICUMAKE="$(ProjectDir)\\\"/NMAKE \/f makedata\.mak ICUMAKE=\"\$(ProjectDir)\\\" VC_VER=\"-vc\$(PlatformToolsetVersion)\"/g' ./data/makedata.vcxproj
sed -i 's/ICU_LIB_TARGET=\$(DLL_OUTPUT)\\\$(U_ICUDATA_NAME)\.dll/ICU_LIB_TARGET=\$(DLL_OUTPUT)\\\$(U_ICUDATA_NAME)\$(VC_VER)\.dll/g' ./data/makedata.mak
