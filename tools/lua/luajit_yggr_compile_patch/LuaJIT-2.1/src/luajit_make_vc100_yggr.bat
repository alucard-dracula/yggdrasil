
cd /d %~dp0

:rem mkdir "out_lua_jit210/include"
mkdir "out_lua_jit210/bin"
mkdir "out_lua_jit210/bin64"
mkdir "out_lua_jit210/exe32/vc100"
mkdir "out_lua_jit210/exe64/vc100"
mkdir "out_lua_jit210/lib"
mkdir "out_lua_jit210/lib64"

call "D:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" amd64

call msvcbuild_yggr.bat vc100 debug
call msvcbuild_yggr.bat vc100 debug static

if exist "out_lua_jit210\include" ( goto not_cp_inc )
mkdir "out_lua_jit210/include"
cp -f *.h "out_lua_jit210/include/"
:not_cp_inc

rm -f luajit.exe

call msvcbuild_yggr.bat vc100 
call msvcbuild_yggr.bat vc100 static

cp -f luajit.exe out_lua_jit210/exe64/vc100/lua.exe
cp -f luajit.exe out_lua_jit210/exe64/vc100/luac.exe
mv -f luajit.exe out_lua_jit210/exe64/vc100/luajit.exe

mv -f luajit501-vc100-d.dll out_lua_jit210/bin64/luajit501-vc100-d.dll
mv -f luajit501-vc100.dll out_lua_jit210/bin64/luajit501-vc100.dll

mv -f luajit501-vc100-d.pdb out_lua_jit210/bin64/luajit501-vc100-d.pdb

mv -f libluajit501-vc100-d.lib out_lua_jit210/lib64/libluajit501-vc100-d.lib
mv -f libluajit501-vc100.lib out_lua_jit210/lib64/libluajit501-vc100.lib

mv -f libluajit501-vc100-d.exp out_lua_jit210/lib64/libluajit501-vc100-d.exp
mv -f libluajit501-vc100.exp out_lua_jit210/lib64/libluajit501-vc100.exp

mv -f libluajit501-vc100-s-d.lib out_lua_jit210/lib64/libluajit501-vc100-s-d.lib
mv -f libluajit501-vc100-s.lib out_lua_jit210/lib64/libluajit501-vc100-s.lib

rm -f *.pdb

call "D:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86

call msvcbuild_yggr.bat vc100 debug
call msvcbuild_yggr.bat vc100 debug static

:rem cp -f luajit.h out_lua_jit210/include/luajit.h.vc100-32

rm -f luajit.exe

call msvcbuild_yggr.bat vc100
call msvcbuild_yggr.bat vc100 static

cp -f luajit.exe out_lua_jit210/exe32/vc100/lua.exe
cp -f luajit.exe out_lua_jit210/exe32/vc100/luac.exe
mv -f luajit.exe out_lua_jit210/exe32/vc100/luajit.exe

mv -f luajit501-vc100-d.dll out_lua_jit210/bin/luajit501-vc100-d.dll
mv -f luajit501-vc100.dll out_lua_jit210/bin/luajit501-vc100.dll

mv -f luajit501-vc100-d.pdb out_lua_jit210/bin/luajit501-vc100-d.pdb

mv -f libluajit501-vc100-d.lib out_lua_jit210/lib/libluajit501-vc100-d.lib
mv -f libluajit501-vc100.lib out_lua_jit210/lib/libluajit501-vc100.lib

mv -f libluajit501-vc100-d.exp out_lua_jit210/lib/libluajit501-vc100-d.exp
mv -f libluajit501-vc100.exp out_lua_jit210/lib/libluajit501-vc100.exp

mv -f libluajit501-vc100-s-d.lib out_lua_jit210/lib/libluajit501-vc100-s-d.lib
mv -f libluajit501-vc100-s.lib out_lua_jit210/lib/libluajit501-vc100-s.lib

rm -f *.pdb