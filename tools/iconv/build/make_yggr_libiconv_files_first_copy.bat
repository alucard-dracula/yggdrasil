cd /d %~dp0

set var_src_root_dir=%~1
set var_dst_root_dir=%~2

:rem (e.g. libiconv)
set var_dst_dir_title=%~3

:rem (e.g. v1.17)
set var_iconv_ver=%~4

:rem (e.g. vc100 vc143)
set var_cl_mark=%~5

:rem (e.g. vc100 vc143 mix)
set var_dst_dir_mark=%~6

:rem (e.g. x64 x32)
set var_platfrom=%~7

:rem (e.g. debug release)
set var_dr_ver=%~8

:rem (e.g. "" mt)
set var_mt_ver=%~9

set var_lib_ext_name=.lib
set var_impl_lib_ext_name=.lib

set var_debug_mark=
if "%var_dr_ver%"=="debug" set var_debug_mark=-d

set var_mt_mark=
set var_mt_dir_mark=

if "%var_mt_ver%"=="mt" set var_mt_mark=-s
if "%var_mt_ver%"=="mt" set var_mt_dir_mark=-mt

set var_proj_dir=

if "%var_cl_mark%"=="vc100" set var_proj_dir=build-VS2010
if "%var_cl_mark%"=="vc143" set var_proj_dir=build-VS2022
if "%var_mt_ver%"=="mt" set var_proj_dir=%var_proj_dir%-MT

set var_build_result_dir=
if "%var_platfrom%"=="x64" set var_build_result_dir=x64/
if "%var_dr_ver%"=="debug" set var_build_result_dir=%var_build_result_dir%Debug
if "%var_dr_ver%"=="release" set var_build_result_dir=%var_build_result_dir%Release

set var_dst_bin_dir=bin
set var_dst_lib_dir=lib

if "%var_platfrom%"=="x64" set var_dst_bin_dir=bin64
if "%var_platfrom%"=="x64" set var_dst_lib_dir=lib64

set var_dst_dir=%var_dst_dir_title%-%var_iconv_ver%-%var_dst_dir_mark%
set var_src_dir=%var_proj_dir%/%var_build_result_dir%

:rem congig and other
touch "%var_dst_root_dir%/%var_dst_dir%/%var_dst_dir%.txt"
cp -fr %var_src_root_dir%/include %var_dst_root_dir%/%var_dst_dir%/

:rem release
cp -f %var_src_root_dir%/%var_proj_dir%/%var_build_result_dir%/*.exe %var_dst_root_dir%/%var_dst_dir%/%var_dst_bin_dir%/
cp -f %var_src_root_dir%/%var_proj_dir%/%var_build_result_dir%/*.dll %var_dst_root_dir%/%var_dst_dir%/%var_dst_bin_dir%/

cp -f ^
    %var_src_root_dir%/%var_proj_dir%/%var_build_result_dir%/*%var_lib_ext_name% ^
    %var_dst_root_dir%/%var_dst_dir%/%var_dst_lib_dir%/

cp -f ^
    %var_src_root_dir%/%var_proj_dir%/%var_build_result_dir%/libiconv%var_impl_lib_ext_name% ^
    %var_dst_root_dir%/%var_dst_dir%/%var_dst_lib_dir%/libiconv-%var_cl_mark%%var_mt_mark%%var_debug_mark%%var_impl_lib_ext_name%

cp -f ^
    %var_src_root_dir%/%var_proj_dir%/%var_build_result_dir%/libiconv-static%var_lib_ext_name% ^
    %var_dst_root_dir%/%var_dst_dir%/%var_dst_lib_dir%/libiconv-static-%var_cl_mark%%var_mt_mark%%var_debug_mark%%var_lib_ext_name%
