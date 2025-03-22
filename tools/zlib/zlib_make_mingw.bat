cd /d %~dp0

set org_path=%path%
:rem x64
set path=%mingw64_path%;%org_path%
:rem gcc -v

:rem debug64
mingw32-make -f win32/Makefile.gcc all CFLAGS="-DZLIB_DEBUG -m64 -g -Wall" LDFLAGS="-m64" SHAREDLIB="zlib1-mgw14-d.dll" IMPLIB="libz-mgw14-d.dll.a" STATICLIB="libz-mgw14-d.a"
call zlib_copy_files_tpl.bat zlib1-mgw14-d.dll libz-mgw14-d.dll.a libz-mgw14-d.a x64
mingw32-make -f win32/Makefile.gcc clean SHAREDLIB="zlib1-mgw14-d.dll" IMPLIB="libz-mgw14-d.dll.a" STATICLIB="libz-mgw14-d.a"

:rem debug64 org_name
mingw32-make -f win32/Makefile.gcc all CFLAGS="-DZLIB_DEBUG -m64 -g -Wall" LDFLAGS="-m64" SHAREDLIB="zlib1d.dll" IMPLIB="libzd.dll.a" STATICLIB="libzd.a"
call zlib_copy_files_tpl.bat zlib1d.dll libzd.dll.a libzd.a x64
mingw32-make -f win32/Makefile.gcc clean SHAREDLIB="zlib1d.dll" IMPLIB="libzd.dll.a" STATICLIB="libzd.a"

:rem release64
mingw32-make -f win32/Makefile.gcc all CFLAGS="-m64 -O3 -Wall" LDFLAGS="-m64 -s" SHAREDLIB="zlib1-mgw14.dll" IMPLIB="libz-mgw14.dll.a" STATICLIB="libz-mgw14.a"
call zlib_copy_files_tpl.bat zlib1-mgw14.dll libz-mgw14.dll.a libz-mgw14.a x64
mingw32-make -f win32/Makefile.gcc clean SHAREDLIB="zlib1-mgw14.dll" IMPLIB="libz-mgw14.dll.a" STATICLIB="libz-mgw14.a"

:rem release64 org_name
mingw32-make -f win32/Makefile.gcc all CFLAGS="-m64 -O3 -Wall" LDFLAGS="-m64 -s"
call zlib_copy_files_tpl.bat zlib1.dll libz.dll.a libz.a x64
:rem cp -fr minigzip.exe win32\bin64\minigzip.exe
mingw32-make -f win32/Makefile.gcc clean

:rem x86
set path=%mingw32_path%;%org_path%
:rem gcc -v

:rem debug
mingw32-make -f win32/Makefile.gcc all CFLAGS="-DZLIB_DEBUG -m32 -g -Wall" LDFLAGS="-m32" RCFLAGS="-F pe-i386 --define GCC_WINDRES" SHAREDLIB="zlib1-mgw14-d.dll" IMPLIB="libz-mgw14-d.dll.a" STATICLIB="libz-mgw14-d.a"
call zlib_copy_files_tpl.bat zlib1-mgw14-d.dll libz-mgw14-d.dll.a libz-mgw14-d.a x86
mingw32-make -f win32/Makefile.gcc clean SHAREDLIB="zlib1-mgw14-d.dll" IMPLIB="libz-mgw14-d.dll.a" STATICLIB="libz-mgw14-d.a"

:rem debug org_name
mingw32-make -f win32/Makefile.gcc all CFLAGS="-DZLIB_DEBUG -m32 -g -Wall" LDFLAGS="-m32" RCFLAGS="-F pe-i386 --define GCC_WINDRES" SHAREDLIB="zlib1d.dll" IMPLIB="libzd.dll.a" STATICLIB="libzd.a"
call zlib_copy_files_tpl.bat zlib1d.dll libzd.dll.a libzd.a x86
mingw32-make -f win32/Makefile.gcc clean SHAREDLIB="zlib1d.dll" IMPLIB="libzd.dll.a" STATICLIB="libzd.a"

:rem release
mingw32-make -f win32/Makefile.gcc all CFLAGS="-DZLIB_DEBUG -m32 -O3 -Wall" LDFLAGS="-m32 -s" RCFLAGS="-F pe-i386 --define GCC_WINDRES" SHAREDLIB="zlib1-mgw14.dll" IMPLIB="libz-mgw14.dll.a" STATICLIB="libz-mgw14.a"
call zlib_copy_files_tpl.bat zlib1-mgw14.dll libz-mgw14.dll.a libz-mgw14.a x86
mingw32-make -f win32/Makefile.gcc clean SHAREDLIB="zlib1-mgw14.dll" IMPLIB="libz-mgw14.dll.a" STATICLIB="libz-mgw14.a"

:rem release org_name
mingw32-make -f win32/Makefile.gcc all CFLAGS="-DZLIB_DEBUG -m32 -O3 -Wall" LDFLAGS="-m32 -s" RCFLAGS="-F pe-i386 --define GCC_WINDRES"
call zlib_copy_files_tpl.bat zlib1.dll libz.dll.a libz.a x86
:rem cp -fr minigzip.exe win32\bin\minigzip.exe
mingw32-make -f win32/Makefile.gcc clean


