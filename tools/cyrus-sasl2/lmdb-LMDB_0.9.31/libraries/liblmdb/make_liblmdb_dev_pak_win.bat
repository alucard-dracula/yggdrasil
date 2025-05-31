cd /d %~dp0

set out_dir=stage

rm -fr "%out_dir%"

mkdir "%out_dir%/include"

cp -fr lib64 "%out_dir%/"
cp -fr lib "%out_dir%/"

:rem fix lib64

if exist "%out_dir%/lib64/liblmdb-vc100.lib" goto make_no_vcver_lib64_100

if exist "%out_dir%/lib64/liblmdb-vc143.lib" goto make_no_vcver_lib64_143

:make_no_vcver_lib64_100

cp %out_dir%/lib64/liblmdb-vc100.lib %out_dir%/lib64/liblmdb.lib
goto make_no_vcver_lib64_end

:make_no_vcver_lib64_143

cp "%out_dir%/lib64/liblmdb-vc143.lib" "%out_dir%/lib64/liblmdb.lib"
goto make_no_vcver_lib64_end

:make_no_vcver_lib64_end

if exist "%out_dir%/lib64/liblmdb-mgw14.a" (cp "%out_dir%/lib64/liblmdb-mgw14.a" "%out_dir%/lib64/liblmdb.a")

:rem fix lib

if exist "%out_dir%/lib/liblmdb-vc100.lib" goto make_no_vcver_lib_100

if exist "%out_dir%/lib/liblmdb-vc143.lib" goto make_no_vcver_lib_143

:make_no_vcver_lib_100

cp "%out_dir%/lib/liblmdb-vc100.lib" "%out_dir%/lib/liblmdb.lib"
goto make_no_vcver_lib_end

:make_no_vcver_lib_143

cp "%out_dir%/lib/liblmdb-vc143.lib" "%out_dir%/lib/liblmdb.lib"
goto make_no_vcver_lib_end

:make_no_vcver_lib_end

if exist "%out_dir%/lib/liblmdb-mgw14.a" (cp "%out_dir%/lib/liblmdb-mgw14.a" "%out_dir%/lib/liblmdb.a")


cp "./lmdb.h" "%out_dir%/include/"
cp "./midl.h" "%out_dir%/include/"
