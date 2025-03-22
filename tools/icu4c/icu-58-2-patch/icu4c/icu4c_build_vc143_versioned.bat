
cd /d %~dp0


set var_perfix=libicu_vc143_ver

rm -fr "%var_perfix%"
mkdir "%var_perfix%"


call "d:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
call icu4c_build_msvc64.bat "%var_perfix%"

call "d:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars32.bat"
call icu4c_build_msvc32.bat "%var_perfix%"

cp -fr include "%var_perfix%/"
rm -fr include



