
:rem @echo off
cd /d %~dp0

set android_api=21

set dir_ndk_arm64=arm64-v8a
set dir_clang_arm64=aarch64-linux-android

set dir_ndk_arm32=armeabi-v7a
set dir_clang_arm32=armv7a-linux-androideabi

set dir_ndk_riscv64=riscv64
set dir_clang_riscv64=riscv64-linux-android

set dir_ndk_x86_64=x86_64
set dir_clang_x86_64=x86_64-linux-android

set dir_ndk_x86=x86
set dir_clang_x86=i686-linux-android

cd lib

:rem fix arm64

if not exist %dir_ndk_arm64% goto fix_end_dir_arm64

if not exist %dir_clang_arm64% mkdir %dir_clang_arm64%

cd %dir_clang_arm64%

if exist %android_api% rm -fr %android_api%
mklink /j %android_api% ..\%dir_ndk_arm64%

cd ..

:fix_end_dir_arm64

:rem fix arm32
if not exist %dir_ndk_arm32% goto fix_end_dir_arm32

if not exist %dir_clang_arm32% mkdir %dir_clang_arm32%

cd %dir_clang_arm32%

if exist %android_api% rm -fr %android_api%
mklink /j %android_api% ..\%dir_ndk_arm32%

cd ..

:fix_end_dir_arm32

:rem fix riscv64
if not exist %dir_ndk_riscv64% goto fix_end_dir_riscv64

if not exist %dir_clang_riscv64% mkdir %dir_clang_riscv64%

cd %dir_clang_riscv64%

:rem if exist %android_api% rm -fr %android_api%
:rem mklink /j %android_api% ..\%dir_ndk_riscv64%

if "%android_api%" geq "35" goto fix_api_geq_35
goto fix_api_ngeq_35

:fix_api_geq_35
if exist %android_api% rm -fr %android_api%
mklink /j %android_api% ..\%dir_ndk_riscv64%
goto fix_end_api_geq_35

:fix_api_ngeq_35
if exist 35 rm -fr 35
mklink /j 35 ..\%dir_ndk_riscv64%

:fix_api_gte_35_end

cd ..

:fix_end_dir_riscv64

:rem fix x86_64

if not exist %dir_ndk_x86_64% goto fix_end_dir_x86_64

if not exist %dir_clang_x86_64% mkdir %dir_clang_x86_64%

cd %dir_clang_x86_64%

if exist %android_api% rm -fr %android_api%
mklink /j %android_api% ..\%dir_ndk_x86_64%

cd ..

:fix_end_dir_x86_64

:rem fix x86
if not exist %dir_ndk_x86% goto fix_end_dir_x86

if not exist %dir_clang_x86% mkdir %dir_clang_x86%

cd %dir_clang_x86%

if exist %android_api% rm -fr %android_api%
mklink /j %android_api% ..\%dir_ndk_x86%

cd ..

:fix_end_dir_x86


cd ..
