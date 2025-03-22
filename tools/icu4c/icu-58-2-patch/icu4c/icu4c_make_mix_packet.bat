
cd /d %~dp0

set var_dir_root=libicu_mix

set var_dir_bin64=%var_dir_root%/bin64
set var_dir_lib64=%var_dir_root%/lib64

set var_dir_bin=%var_dir_root%/bin
set var_dir_lib=%var_dir_root%/lib

rm -fr "%var_dir_root%"

mkdir "%var_dir_root%"

mkdir "%var_dir_bin64%"
mkdir "%var_dir_lib64%"

mkdir "%var_dir_bin%"
mkdir "%var_dir_lib%"


cp -fr libicu_vc100_ver/include %var_dir_root%/

cp -f libicu_vc100_ver/bin64/* %var_dir_bin64%/
cp -f libicu_vc100_ver/lib64/* %var_dir_lib64%/
cp -f libicu_vc100_ver/bin/* %var_dir_bin%/
cp -f libicu_vc100_ver/lib/* %var_dir_lib%/

cp -f libicu_vc143_ver/bin64/* %var_dir_bin64%/
cp -f libicu_vc143_ver/lib64/* %var_dir_lib64%/
cp -f libicu_vc143_ver/bin/* %var_dir_bin%/
cp -f libicu_vc143_ver/lib/* %var_dir_lib%/

cp -f libicu_mgw14_x64/lib/*.dll %var_dir_bin64%/
cp -f libicu_mgw14_x64/lib/*.a %var_dir_lib64%/

cp -f libicu_mgw14_x32/lib/*.dll %var_dir_bin%/
cp -f libicu_mgw14_x32/lib/*.a %var_dir_lib%/

touch %var_dir_root%/libicu_mix.txt





