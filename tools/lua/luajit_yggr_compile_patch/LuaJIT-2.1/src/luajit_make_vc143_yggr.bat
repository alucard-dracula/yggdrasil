
cd /d %~dp0

:rem mkdir "out_lua_jit210/include"
mkdir "out_lua_jit210/bin"
mkdir "out_lua_jit210/bin64"
mkdir "out_lua_jit210/exe32/vc143"
mkdir "out_lua_jit210/exe64/vc143"
mkdir "out_lua_jit210/lib"
mkdir "out_lua_jit210/lib64"

call "d:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

call msvcbuild_yggr.bat vc143 debug
call msvcbuild_yggr.bat vc143 debug static

if exist "out_lua_jit210\include" ( goto not_cp_inc )
mkdir "out_lua_jit210/include"
cp -f *.h "out_lua_jit210/include/"
:not_cp_inc

rm -f luajit.exe

call msvcbuild_yggr.bat vc143
call msvcbuild_yggr.bat vc143 static

cp -f luajit.exe out_lua_jit210/exe64/vc143/lua.exe
cp -f luajit.exe out_lua_jit210/exe64/vc143/luac.exe
mv -f luajit.exe out_lua_jit210/exe64/vc143/luajit.exe

mv -f luajit501-vc143-d.dll out_lua_jit210/bin64/luajit501-vc143-d.dll
mv -f luajit501-vc143.dll out_lua_jit210/bin64/luajit501-vc143.dll

mv -f luajit501-vc143-d.pdb out_lua_jit210/bin64/luajit501-vc143-d.pdb

mv -f libluajit501-vc143-d.lib out_lua_jit210/lib64/libluajit501-vc143-d.lib
mv -f libluajit501-vc143.lib out_lua_jit210/lib64/libluajit501-vc143.lib

mv -f libluajit501-vc143-d.exp out_lua_jit210/lib64/libluajit501-vc143-d.exp
mv -f libluajit501-vc143.exp out_lua_jit210/lib64/libluajit501-vc143.exp

mv -f libluajit501-vc143-s-d.lib out_lua_jit210/lib64/libluajit501-vc143-s-d.lib
mv -f libluajit501-vc143-s.lib out_lua_jit210/lib64/libluajit501-vc143-s.lib

rm -f *.pdb

call "d:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars32.bat"

call msvcbuild_yggr.bat vc143 debug
call msvcbuild_yggr.bat vc143 debug static

:rem cp -f luajit.h out_lua_jit210/include/luajit.h.vc143-32
rm -f luajit.exe

call msvcbuild_yggr.bat vc143
call msvcbuild_yggr.bat vc143 static

cp -f luajit.exe out_lua_jit210/exe32/vc143/lua.exe
cp -f luajit.exe out_lua_jit210/exe32/vc143/luac.exe
mv -f luajit.exe out_lua_jit210/exe32/vc143/luajit.exe

mv -f luajit501-vc143-d.dll out_lua_jit210/bin/luajit501-vc143-d.dll
mv -f luajit501-vc143.dll out_lua_jit210/bin/luajit501-vc143.dll

mv -f luajit501-vc143-d.pdb out_lua_jit210/bin/luajit501-vc143-d.pdb

mv -f libluajit501-vc143-d.lib out_lua_jit210/lib/libluajit501-vc143-d.lib
mv -f libluajit501-vc143.lib out_lua_jit210/lib/libluajit501-vc143.lib

mv -f libluajit501-vc143-d.exp out_lua_jit210/lib/libluajit501-vc143-d.exp
mv -f libluajit501-vc143.exp out_lua_jit210/lib/libluajit501-vc143.exp

mv -f libluajit501-vc143-s-d.lib out_lua_jit210/lib/libluajit501-vc143-s-d.lib
mv -f libluajit501-vc143-s.lib out_lua_jit210/lib/libluajit501-vc143-s.lib

rm -f *.pdb