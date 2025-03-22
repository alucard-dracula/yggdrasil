
call "d:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

cd /d %~dp0

set src_dir64="mingw-w64-master/mingw-w64-crt/lib64/"
set src_dir32="mingw-w64-master/mingw-w64-crt/lib32/"

set dst_root_dir="mingw_win_impllib_ex"
set dst_dir64="%dst_root_dir%/lib64/"
set dst_dir32="%dst_root_dir%/lib/"

rm -fr %dst_root_dir%

mkdir %dst_dir64%
mkdir %dst_dir32%

lib /def:%src_dir64%bcrypt.def /machine:x64 /out:%dst_dir64%libbcrypt.a
lib /def:%src_dir64%bcrypt.def /machine:x86 /out:%dst_dir32%libbcrypt.a

lib /def:%src_dir64%crypt32.def /machine:x64 /out:%dst_dir64%libcrypt32.a
lib /def:%src_dir64%crypt32.def /machine:x86 /out:%dst_dir32%libcrypt32.a

lib /def:%src_dir64%dnsapi.def /machine:x64 /out:%dst_dir64%libdnsapi.a
lib /def:%src_dir64%dnsapi.def /machine:x86 /out:%dst_dir32%libdnsapi.a

lib /def:%src_dir64%secur32.def /machine:x64 /out:%dst_dir64%libsecur32.a
lib /def:%src_dir64%secur32.def /machine:x86 /out:%dst_dir32%libsecur32.a

lib /def:%src_dir64%kernel32.def /machine:x64 /out:%dst_dir64%libkernel32.a
lib /def:%src_dir64%kernel32.def /machine:x86 /out:%dst_dir32%libkernel32.a

lib /def:%src_dir64%ws2_32.def /machine:x64 /out:%dst_dir64%libws2_32.a
lib /def:%src_dir64%ws2_32.def /machine:x86 /out:%dst_dir32%libws2_32.a

