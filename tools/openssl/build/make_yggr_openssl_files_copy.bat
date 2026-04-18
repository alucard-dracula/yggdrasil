cd /d %~dp0

set var_src_root_dir=%~1
set var_dst_root_dir=%~2

:rem (e.g. libopenssl)
set var_dst_dir_title=%~3

:rem (e.g. 3.4.0)
set var_ssl_ver=%~4

:rem (e.g. vc100 vc143 mgw15)
set var_cl_mark=%~5

:rem (e.g. vc100 vc143 mgw15 mix)
set var_dst_dir_mark=%~6

:rem (e.g. x64 x32)
set var_platfrom=%~7

:rem (e.g. debug release)
set var_dr_ver=%~8

:rem (e.g. "" mt)
set var_mt_ver=%~9

set var_lib_ext_name=.lib
set var_impl_lib_ext_name=.lib
if "%var_cl_mark:~0,3%"=="mgw" set var_lib_ext_name=.a
if "%var_cl_mark:~0,3%"=="mgw" set var_impl_lib_ext_name=.dll.a

set var_debug_mark=
if "%var_dr_ver%"=="debug" set var_debug_mark=-d


set var_mt_mark=
set var_mt_dir_mark=

if "%var_mt_ver%"=="mt" set var_mt_mark=-s
if "%var_mt_ver%"=="mt" set var_mt_dir_mark=-mt

set var_src_bin_dir=bin
set var_src_lib_dir=lib

if "%var_cl_mark:~0,3%" equ "mgw" if "%var_platfrom%" equ "x64" set var_src_lib_dir=lib64

set var_dst_bin_dir=bin
set var_dst_lib_dir=lib

set var_dll_platfrom_mark=
set var_platfrom_cl=x64

if "%var_platfrom%"=="x64" set var_dst_bin_dir=bin64
if "%var_platfrom%"=="x64" set var_dst_lib_dir=lib64
if "%var_platfrom%"=="x64" set var_dll_platfrom_mark=-x64

if "%var_platfrom%"=="x32" set var_platfrom_cl=x86

if "%var_cl_mark:~0,3%" neq "mgw" goto :copy_start

if "%var_platfrom%"=="x64" set var_platfrom_cl=x86-64
if "%var_platfrom%"=="x32" set var_platfrom_cl=i386

:copy_start

set var_dst_dir=%var_dst_dir_title%-%var_ssl_ver%-%var_dst_dir_mark%
set var_src_dir=%var_cl_mark%-%var_platfrom%-%var_dr_ver%%var_mt_dir_mark%
set var_src_static_dir=%var_cl_mark%-%var_platfrom%-static-%var_dr_ver%%var_mt_dir_mark%

if exist %var_dst_root_dir%/%var_dst_dir%/%var_dst_bin_dir%/c_rehash goto :copy_start2

if "%var_cl_mark:~0,3%"=="mgw" goto :copy_c_rehash_mgw

goto :copy_start2

:copy_c_rehash_mgw
cp -f ^
    %var_src_root_dir%/%var_src_dir%/bin/c_rehash ^
    %var_dst_root_dir%/%var_dst_dir%/%var_dst_bin_dir%/c_rehash

:copy_start2

if exist %var_dst_root_dir%/%var_dst_dir%/%var_dst_bin_dir%/c_rehash.pl goto :copy_start3

if "%var_cl_mark:~0,2%"=="vc" goto :copy_c_rehash_vc

goto :copy_start3

:copy_c_rehash_vc
cp -f ^
    %var_src_root_dir%/%var_src_dir%/bin/c_rehash.pl ^
    %var_dst_root_dir%/%var_dst_dir%/%var_dst_bin_dir%/c_rehash.pl


:copy_start3

cp -f ^
    %var_src_root_dir%/%var_src_dir%/bin/libcrypto-3%var_dll_platfrom_mark%.dll ^
    %var_dst_root_dir%/%var_dst_dir%/%var_dst_bin_dir%/libcrypto-3%var_dll_platfrom_mark%-%var_cl_mark%%var_mt_mark%%var_debug_mark%.dll

cp -f ^
    %var_src_root_dir%/%var_src_dir%/bin/libssl-3%var_dll_platfrom_mark%.dll ^
    %var_dst_root_dir%/%var_dst_dir%/%var_dst_bin_dir%/libssl-3%var_dll_platfrom_mark%-%var_cl_mark%%var_mt_mark%%var_debug_mark%.dll


call rebuild_impl_lib.bat ^
    "%var_dst_root_dir%/%var_dst_dir%/%var_dst_bin_dir%" ^
    "libcrypto-3%var_dll_platfrom_mark%-%var_cl_mark%%var_mt_mark%%var_debug_mark%.dll" ^
    "%var_dst_root_dir%/%var_dst_dir%/%var_dst_lib_dir%" ^
    "libcrypto-%var_cl_mark%%var_mt_mark%%var_debug_mark%%var_impl_lib_ext_name%" ^
    "libcrypto-3%var_dll_platfrom_mark%-%var_cl_mark%%var_mt_mark%%var_debug_mark%" ^
    "%var_platfrom_cl%" "has_env"

call rebuild_impl_lib.bat ^
    "%var_dst_root_dir%/%var_dst_dir%/%var_dst_bin_dir%" ^
    "libssl-3%var_dll_platfrom_mark%-%var_cl_mark%%var_mt_mark%%var_debug_mark%.dll" ^
    "%var_dst_root_dir%/%var_dst_dir%/%var_dst_lib_dir%" ^
    "libssl-%var_cl_mark%%var_mt_mark%%var_debug_mark%%var_impl_lib_ext_name%" ^
    "libssl-3%var_dll_platfrom_mark%-%var_cl_mark%%var_mt_mark%%var_debug_mark%" ^
    "%var_platfrom_cl%" "has_env"

if "%var_cl_mark:~0,3%" neq "mgw" goto :copy_static

cp -f ^
    %var_src_root_dir%/%var_src_static_dir%/%var_src_lib_dir%/libcrypto%var_lib_ext_name% ^
    %var_dst_root_dir%/%var_dst_dir%/%var_dst_lib_dir%/libcrypto_static-%var_cl_mark%%var_mt_mark%%var_debug_mark%%var_lib_ext_name%

cp -f ^
    %var_src_root_dir%/%var_src_static_dir%/%var_src_lib_dir%/libssl%var_lib_ext_name% ^
    %var_dst_root_dir%/%var_dst_dir%/%var_dst_lib_dir%/libssl_static-%var_cl_mark%%var_mt_mark%%var_debug_mark%%var_lib_ext_name%

:copy_static
cp -f ^
    %var_src_root_dir%/%var_src_static_dir%/%var_src_lib_dir%/libcrypto%var_lib_ext_name% ^
    %var_dst_root_dir%/%var_dst_dir%/%var_dst_lib_dir%/libcrypto_static-%var_cl_mark%%var_mt_mark%%var_debug_mark%%var_lib_ext_name%

cp -f ^
    %var_src_root_dir%/%var_src_static_dir%/%var_src_lib_dir%/libssl%var_lib_ext_name% ^
    %var_dst_root_dir%/%var_dst_dir%/%var_dst_lib_dir%/libssl_static-%var_cl_mark%%var_mt_mark%%var_debug_mark%%var_lib_ext_name%

