
:rem zlib_copy_files_tpl version (e.g zlib_copy_files_tpl vc100)
:rem @echo off

cd /d %~dp0

set bin_dir=win32\bin64\
set lib_dir=win32\lib64\
set inc_dir=win32\include\

if "%4"=="x86" set bin_dir=win32\bin\
if "%4"=="x86" set lib_dir=win32\lib\


if not exist %bin_dir% md %bin_dir%
if not exist %lib_dir% md %lib_dir%

:rem src file
:rem dll
set var_f_dll_zlib=%1

:rem dll.lib
set var_f_dlib_zlib=%2

:rem lib
set var_f_lib_zlib=%3


if exist %var_f_dll_zlib% cp -f %var_f_dll_zlib% %bin_dir%%var_f_dll_zlib%

:rem dll.lib
if exist %var_f_dlib_zlib% cp -f %var_f_dlib_zlib% %lib_dir%%var_f_dlib_zlib%

:rem lib
if exist %var_f_lib_zlib% cp -f %var_f_lib_zlib% %lib_dir%%var_f_lib_zlib%


:rem inclue

if exist %inc_dir% goto end

md %inc_dir%
if exist crc32.h cp -f crc32.h %inc_dir%crc32.h
if exist deflate.h cp -f deflate.h %inc_dir%deflate.h
if exist gzguts.h cp -f gzguts.h %inc_dir%gzguts.h
if exist inffast.h cp -f inffast.h %inc_dir%inffast.h
if exist inffixed.h cp -f inffixed.h %inc_dir%inffixed.h
if exist inflate.h cp -f inflate.h %inc_dir%inflate.h
if exist inftrees.h cp -f inftrees.h %inc_dir%inftrees.h
if exist trees.h cp -f trees.h %inc_dir%trees.h
if exist zconf.h cp -f zconf.h %inc_dir%zconf.h
if exist zlib.h cp -f zlib.h %inc_dir%zlib.h
if exist zutil.h cp -f zutil.h %inc_dir%zutil.h

:end

