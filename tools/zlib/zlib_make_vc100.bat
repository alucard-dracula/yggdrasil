
:rem x64

call "D:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" amd64

cd /d %~dp0
:rem nmake /help

:rem debug64
nmake -f win32/Makefile.msc all LOC="-DZLIB_DEBUG -D_DEBUG -DWIN64 -Od" SHAREDLIB="zlib-vc100-d.dll" IMPLIB="libzlib-vc100-d.lib" STATICLIB="libzlibstatic-vc100-d.lib"
call zlib_copy_files_tpl.bat zlib-vc100-d.dll libzlib-vc100-d.lib libzlibstatic-vc100-d.lib x64
nmake -f win32/Makefile.msc clean SHAREDLIB="zlib-vc100-d.dll" IMPLIB="libzlib-vc100-d.lib" STATICLIB="libzlibstatic-vc100-d.lib"

:rem debug64 org_name
nmake -f win32/Makefile.msc all LOC="-DZLIB_DEBUG -D_DEBUG -DWIN64 -Od" SHAREDLIB="zlibd.dll" IMPLIB="zlibd.lib" STATICLIB="zlibstaticd.lib"
call zlib_copy_files_tpl.bat zlibd.dll zlibd.lib zlibstaticd.lib x64
nmake -f win32/Makefile.msc clean SHAREDLIB="zlibd.dll" IMPLIB="zlibd.lib" STATICLIB="zlibstaticd.lib"

:rem release64
nmake -f win32/Makefile.msc all LOC="-DNDEBUG -DWIN64 -O2" SHAREDLIB="zlib-vc100.dll" IMPLIB="libzlib-vc100.lib" STATICLIB="libzlibstatic-vc100.lib"
call zlib_copy_files_tpl.bat zlib-vc100.dll libzlib-vc100.lib libzlibstatic-vc100.lib x64
nmake -f win32/Makefile.msc clean SHAREDLIB="zlib-vc100.dll" IMPLIB="libzlib-vc100.lib" STATICLIB="libzlibstatic-vc100.lib"

:rem release64 org_name
nmake -f win32/Makefile.msc all LOC="-DNDEBUG -DWIN64 -O2" SHAREDLIB="zlib.dll" IMPLIB="zlib.lib" STATICLIB="zlibstatic.lib"
call zlib_copy_files_tpl.bat zlib.dll zlib.lib zlibstatic.lib x64
:rem cp -fr minigzip.exe win32\bin64\minigzip.exe
nmake -f win32/Makefile.msc clean SHAREDLIB="zlib.dll" IMPLIB="zlib.lib" STATICLIB="zlibstatic.lib"

:rem x86

call "D:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
:rem nmake /help

:rem debug
nmake -f win32/Makefile.msc all LOC="-DZLIB_DEBUG -D_DEBUG -Od" SHAREDLIB="zlib-vc100-d.dll" IMPLIB="libzlib-vc100-d.lib" STATICLIB="libzlibstatic-vc100-d.lib"
call zlib_copy_files_tpl.bat zlib-vc100-d.dll libzlib-vc100-d.lib libzlibstatic-vc100-d.lib x86
nmake -f win32/Makefile.msc clean SHAREDLIB="zlib-vc100-d.dll" IMPLIB="libzlib-vc100-d.lib" STATICLIB="libzlibstatic-vc100-d.lib"

:rem debug org_name
nmake -f win32/Makefile.msc all LOC="-DZLIB_DEBUG -D_DEBUG -Od" SHAREDLIB="zlibd.dll" IMPLIB="zlibd.lib" STATICLIB="zlibstaticd.lib"
call zlib_copy_files_tpl.bat zlibd.dll zlibd.lib zlibstaticd.lib x86
nmake -f win32/Makefile.msc clean SHAREDLIB="zlibd.dll" IMPLIB="zlibd.lib" STATICLIB="zlibstaticd.lib"

:rem release
nmake -f win32/Makefile.msc all LOC="-DNDEBUG -O2" SHAREDLIB="zlib-vc100.dll" IMPLIB="libzlib-vc100.lib" STATICLIB="libzlibstatic-vc100.lib"
call zlib_copy_files_tpl.bat zlib-vc100.dll libzlib-vc100.lib libzlibstatic-vc100.lib x86
nmake -f win32/Makefile.msc clean SHAREDLIB="zlib-vc100.dll" IMPLIB="libzlib-vc100.lib" STATICLIB="libzlibstatic-vc100.lib"

:rem release org_name
nmake -f win32/Makefile.msc all LOC="-DNDEBUG -O2" SHAREDLIB="zlib.dll" IMPLIB="zlib.lib" STATICLIB="zlibstatic.lib"
call zlib_copy_files_tpl.bat zlib.dll zlib.lib zlibstatic.lib x86
:rem cp -fr minigzip.exe win32\bin\minigzip.exe
nmake -f win32/Makefile.msc clean SHAREDLIB="zlib.dll" IMPLIB="zlib.lib" STATICLIB="zlibstatic.lib"
