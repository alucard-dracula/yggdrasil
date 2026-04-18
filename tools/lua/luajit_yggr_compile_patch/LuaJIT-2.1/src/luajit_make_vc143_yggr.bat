
cd /d %~dp0

:rem mkdir "out_lua_jit210/include"
mkdir "out_lua_jit210/bin"
mkdir "out_lua_jit210/bin64"
mkdir "out_lua_jit210/exe32/vc143"
mkdir "out_lua_jit210/exe64/vc143"
mkdir "out_lua_jit210/lib"
mkdir "out_lua_jit210/lib64"

setlocal

call "d:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

call msvcbuild_yggr.bat vc143 debug-mt static
call msvcbuild_yggr.bat vc143 debug-mt

call msvcbuild_yggr.bat vc143 debug static
call msvcbuild_yggr.bat vc143 debug

if exist "out_lua_jit210\include" ( goto not_cp_inc )
mkdir "out_lua_jit210/include"
cp -f *.h "out_lua_jit210/include/"
cp -f *.hpp "out_lua_jit210/include/"
:not_cp_inc

rm -f luajit.exe
rm -f luajit-s.exe

call msvcbuild_yggr.bat vc143 release-mt
call msvcbuild_yggr.bat vc143 release-mt static

call msvcbuild_yggr.bat vc143 release static
call msvcbuild_yggr.bat vc143 release

cp -f luajit.exe out_lua_jit210/exe64/vc143/lua.exe
cp -f luajit.exe out_lua_jit210/exe64/vc143/luac.exe
mv -f luajit.exe out_lua_jit210/exe64/vc143/luajit.exe
mv -f luajit-s.exe out_lua_jit210/exe64/vc143/luajit-s.exe

:rem dll
mv -f luajit501-vc143-d.dll out_lua_jit210/bin64/luajit501-vc143-d.dll
mv -f luajit501-vc143.dll out_lua_jit210/bin64/luajit501-vc143.dll

mv -f luajit501-vc143-d.pdb out_lua_jit210/bin64/luajit501-vc143-d.pdb

mv -f luajit501-vc143-d.lib out_lua_jit210/lib64/luajit501-vc143-d.lib
mv -f luajit501-vc143.lib out_lua_jit210/lib64/luajit501-vc143.lib

mv -f luajit501-vc143-d.exp out_lua_jit210/lib64/luajit501-vc143-d.exp
mv -f luajit501-vc143.exp out_lua_jit210/lib64/luajit501-vc143.exp

:rem dll-mt
mv -f luajit501-vc143-s-d.dll out_lua_jit210/bin64/luajit501-vc143-s-d.dll
mv -f luajit501-vc143-s.dll out_lua_jit210/bin64/luajit501-vc143-s.dll

mv -f luajit501-vc143-s-d.pdb out_lua_jit210/bin64/luajit501-vc143-s-d.pdb

mv -f luajit501-vc143-s-d.lib out_lua_jit210/lib64/luajit501-vc143-s-d.lib
mv -f luajit501-vc143-s.lib out_lua_jit210/lib64/luajit501-vc143-s.lib

mv -f luajit501-vc143-s-d.exp out_lua_jit210/lib64/luajit501-vc143-s-d.exp
mv -f luajit501-vc143-s.exp out_lua_jit210/lib64/luajit501-vc143-s.exp

:rem lib
mv -f libluajit501_static-vc143-d.lib out_lua_jit210/lib64/libluajit501_static-vc143-d.lib
mv -f libluajit501_static-vc143.lib out_lua_jit210/lib64/libluajit501_static-vc143.lib

:rem lib-mt
mv -f libluajit501_static-vc143-s-d.lib out_lua_jit210/lib64/libluajit501_static-vc143-s-d.lib
mv -f libluajit501_static-vc143-s.lib out_lua_jit210/lib64/libluajit501_static-vc143-s.lib

rm -f *.pdb

endlocal

setlocal

call "d:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars32.bat"

call msvcbuild_yggr.bat vc143 debug-mt static
call msvcbuild_yggr.bat vc143 debug-mt

call msvcbuild_yggr.bat vc143 debug static
call msvcbuild_yggr.bat vc143 debug

:rem cp -f luajit.h out_lua_jit210/include/luajit.h.vc143-32

rm -f luajit.exe
rm -f luajit-s.exe

call msvcbuild_yggr.bat vc143 release-mt
call msvcbuild_yggr.bat vc143 release-mt static

call msvcbuild_yggr.bat vc143 release static
call msvcbuild_yggr.bat vc143 release

cp -f luajit.exe out_lua_jit210/exe32/vc143/lua.exe
cp -f luajit.exe out_lua_jit210/exe32/vc143/luac.exe
mv -f luajit.exe out_lua_jit210/exe32/vc143/luajit.exe
mv -f luajit-s.exe out_lua_jit210/exe32/vc143/luajit-s.exe

:rem dll
mv -f luajit501-vc143-d.dll out_lua_jit210/bin/luajit501-vc143-d.dll
mv -f luajit501-vc143.dll out_lua_jit210/bin/luajit501-vc143.dll

mv -f luajit501-vc143-d.pdb out_lua_jit210/bin/luajit501-vc143-d.pdb

mv -f luajit501-vc143-d.lib out_lua_jit210/lib/luajit501-vc143-d.lib
mv -f luajit501-vc143.lib out_lua_jit210/lib/luajit501-vc143.lib

mv -f luajit501-vc143-d.exp out_lua_jit210/lib/luajit501-vc143-d.exp
mv -f luajit501-vc143.exp out_lua_jit210/lib/luajit501-vc143.exp

:rem dll-mt
mv -f luajit501-vc143-s-d.dll out_lua_jit210/bin/luajit501-vc143-s-d.dll
mv -f luajit501-vc143-s.dll out_lua_jit210/bin/luajit501-vc143-s.dll

mv -f luajit501-vc143-s-d.pdb out_lua_jit210/bin/luajit501-vc143-s-d.pdb

mv -f luajit501-vc143-s-d.lib out_lua_jit210/lib/luajit501-vc143-s-d.lib
mv -f luajit501-vc143-s.lib out_lua_jit210/lib/luajit501-vc143-s.lib

mv -f luajit501-vc143-s-d.exp out_lua_jit210/lib/luajit501-vc143-s-d.exp
mv -f luajit501-vc143-s.exp out_lua_jit210/lib/luajit501-vc143-s.exp

:rem lib
mv -f libluajit501_static-vc143-d.lib out_lua_jit210/lib/libluajit501_static-vc143-d.lib
mv -f libluajit501_static-vc143.lib out_lua_jit210/lib/libluajit501_static-vc143.lib

:rem lib-mt
mv -f libluajit501_static-vc143-s-d.lib out_lua_jit210/lib/libluajit501_static-vc143-s-d.lib
mv -f libluajit501_static-vc143-s.lib out_lua_jit210/lib/libluajit501_static-vc143-s.lib

rm -f *.pdb

endlocal

:end