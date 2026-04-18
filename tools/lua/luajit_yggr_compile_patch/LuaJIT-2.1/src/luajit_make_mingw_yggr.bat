
cd /d %~dp0

:rem set path=D:\MinGW64\gdb64\bin;D:\MinGW64\bin;%path%
:rem set path=D:\MinGW32\gdb32\bin;D:\MinGW32\bin;%path%

set org_path=%path%
set path=%mingw64_path%;%org_path%

rm *.exe
rm *.dll
rm *.a
rm *.o
rm host/*.o
rm host/*.exe

:rem mkdir "out_lua_jit210/include"
mkdir "out_lua_jit210/bin"
mkdir "out_lua_jit210/bin64"
mkdir "out_lua_jit210/exe32/mgw15"
mkdir "out_lua_jit210/exe64/mgw15"
mkdir "out_lua_jit210/lib"
mkdir "out_lua_jit210/lib64"

mingw32-make.exe -j12 -f Makefile_yggr NODOTABIVER=jit501-mgw15-d CCDEBUG=-g "-m64"

if exist "out_lua_jit210\include" ( goto not_cp_inc )
mkdir "out_lua_jit210/include"
cp -f *.h "out_lua_jit210/include/"
cp -f *.hpp "out_lua_jit210/include/"
:not_cp_inc


:rem cp -f luajit.h out_lua_jit210/include/luajit.h.mgw15-64

mv -f luajit501-mgw15-d.dll out_lua_jit210/bin64/luajit501-mgw15-d.dll
mv -f libluajit501-mgw15-d.dll.a out_lua_jit210/lib64/libluajit501-mgw15-d.dll.a
cp -f out_lua_jit210/lib64/libluajit501-mgw15-d.dll.a out_lua_jit210/lib64/libluajit501_ts-mgw15-d.dll.a
touch out_lua_jit210/lib64/libluajit501_ts-mgw15-d.dll.a.is_fake

mingw32-make.exe -f Makefile_yggr NODOTABIVER=jit501-mgw15-d clean
rm *.exe
rm *.o
rm host/*.o
rm host/*.exe

mingw32-make.exe -j12 -f Makefile_yggr NODOTABIVER=jit501_static-mgw15-d CCDEBUG=-g BUILDMODE=static "-m64"
mv -f libluajit501_static-mgw15-d.a out_lua_jit210/lib64/libluajit501_static-mgw15-d.a
cp -f out_lua_jit210/lib64/libluajit501_static-mgw15-d.a out_lua_jit210/lib64/libluajit501_ts_static-mgw15-d.a
touch out_lua_jit210/lib64/libluajit501_ts_static-mgw15-d.a.is_fake

mingw32-make.exe -f Makefile_yggr NODOTABIVER=jit501_static-mgw15-d clean
rm *.exe
rm *.o
rm host/*.o
rm host/*.exe

mingw32-make.exe -j12 -f Makefile_yggr NODOTABIVER=jit501-mgw15 CFLAGS=-O3 LDFLAGS=-s "-m64"
mv -f luajit501-mgw15.dll out_lua_jit210/bin64/luajit501-mgw15.dll
mv -f libluajit501-mgw15.dll.a out_lua_jit210/lib64/libluajit501-mgw15.dll.a
cp -f out_lua_jit210/lib64/libluajit501-mgw15.dll.a out_lua_jit210/lib64/libluajit501_ts-mgw15.dll.a
touch out_lua_jit210/lib64/libluajit501_ts-mgw15.dll.a.is_fake

cp -f luajit.exe out_lua_jit210/exe64/mgw15/lua.exe
mv -f luajit.exe out_lua_jit210/exe64/mgw15/luajit.exe

mingw32-make.exe -f Makefile_yggr NODOTABIVER=jit501-mgw15 clean
rm *.exe
rm *.o
rm host/*.o
rm host/*.exe

mingw32-make.exe -j12 -f Makefile_yggr NODOTABIVER=jit501_static-mgw15 CFLAGS=-O3 LDFLAGS=-s BUILDMODE=static "-m64"
mv -f libluajit501_static-mgw15.a out_lua_jit210/lib64/libluajit501_static-mgw15.a
cp -f out_lua_jit210/lib64/libluajit501_static-mgw15.a out_lua_jit210/lib64/libluajit501_ts_static-mgw15.a
touch out_lua_jit210/lib64/libluajit501_ts_static-mgw15.a.is_fake

cp -f luajit.exe out_lua_jit210/exe64/mgw15/luac.exe

mingw32-make.exe -f Makefile_yggr NODOTABIVER=jit501_static-mgw15 clean
rm *.o
rm host/*.o
rm host/*.exe

#set path=%mingw32_path%;%org_path%

mingw32-make.exe -j12 -f Makefile_yggr NODOTABIVER=jit501-mgw15-d CCDEBUG=-g CC="$(DEFAULT_CC) -m32"

:rem cp -f luajit.h out_lua_jit210/include/luajit.h.mgw15-32

mv -f luajit501-mgw15-d.dll out_lua_jit210/bin/luajit501-mgw15-d.dll
mv -f libluajit501-mgw15-d.dll.a out_lua_jit210/lib/libluajit501-mgw15-d.dll.a
cp -f out_lua_jit210/lib/libluajit501-mgw15-d.dll.a out_lua_jit210/lib/libluajit501_ts-mgw15-d.dll.a
touch out_lua_jit210/lib/libluajit501_ts-mgw15-d.dll.a.is_fake

mingw32-make.exe -f Makefile_yggr NODOTABIVER=jit501-mgw15-d clean
rm *.exe
rm *.o
rm host/*.o
rm host/*.exe

mingw32-make.exe -j12 -f Makefile_yggr NODOTABIVER=jit501_static-mgw15-d CCDEBUG=-g CC="$(DEFAULT_CC) -m32" BUILDMODE=static
mv -f libluajit501_static-mgw15-d.a out_lua_jit210/lib/libluajit501_static-mgw15-d.a
cp -fr out_lua_jit210/lib/libluajit501_static-mgw15-d.a out_lua_jit210/lib/libluajit501_ts_static-mgw15-d.a
touch out_lua_jit210/lib/libluajit501_ts_static-mgw15-d.a.is_fake

mingw32-make.exe -f Makefile_yggr NODOTABIVER=jit501_static-mgw15-d clean
rm *.exe
rm *.o
rm host/*.o
rm host/*.exe

mingw32-make.exe -j12 -f Makefile_yggr NODOTABIVER=jit501-mgw15 CFLAGS=-O3 CC="$(DEFAULT_CC) -m32" LDFLAGS=-s
mv -f luajit501-mgw15.dll out_lua_jit210/bin/luajit501-mgw15.dll
mv -f libluajit501-mgw15.dll.a out_lua_jit210/lib/libluajit501-mgw15.dll.a
cp -f out_lua_jit210/lib/libluajit501-mgw15.dll.a out_lua_jit210/lib/libluajit501_ts-mgw15.dll.a
touch out_lua_jit210/lib/libluajit501_ts-mgw15.dll.a.is_fake

cp -f luajit.exe out_lua_jit210/exe32/mgw15/lua.exe
mv -f luajit.exe out_lua_jit210/exe32/mgw15/luajit.exe

mingw32-make.exe -f Makefile_yggr NODOTABIVER=jit501-mgw15 clean
rm *.exe
rm *.o
rm host/*.o
rm host/*.exe

mingw32-make.exe -j12 -f Makefile_yggr NODOTABIVER=jit501_static-mgw15 CFLAGS=-O3 CC="$(DEFAULT_CC) -m32" LDFLAGS=-s BUILDMODE=static
mv -f libluajit501_static-mgw15.a out_lua_jit210/lib/libluajit501_static-mgw15.a
cp -f out_lua_jit210/lib/libluajit501_static-mgw15.a out_lua_jit210/lib/libluajit501_ts_static-mgw15.a
touch out_lua_jit210/lib/libluajit501_ts_static-mgw15.a.is_fake

cp -f luajit.exe out_lua_jit210/exe32/mgw15/luac.exe

mingw32-make.exe -f Makefile_yggr NODOTABIVER=jit501_static-mgw15 clean
rm *.o
rm host/*.o
rm host/*.exe

set path=%org_path%
