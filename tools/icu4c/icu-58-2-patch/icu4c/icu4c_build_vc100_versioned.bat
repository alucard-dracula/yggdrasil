
cd /d %~dp0

set var_perfix=libicu_vc100_ver

rm -fr "%var_perfix%"
mkdir "%var_perfix%"

call "D:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" amd64
call icu4c_build_msvc64.bat "%var_perfix%"

call "D:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
call icu4c_build_msvc32.bat "%var_perfix%"

cp -fr include "%var_perfix%/"
rm -fr include



