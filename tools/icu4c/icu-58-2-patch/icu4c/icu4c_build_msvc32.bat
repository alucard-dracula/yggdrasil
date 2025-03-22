

cd /d %~dp0

set var_outdir=%1

mkdir "./%var_outdir%/bin"
mkdir "./%var_outdir%/lib"


msbuild source/allinone/allinone.sln /m:12 /p:Configuration=Debug /p:Platform=Win32

cp -fr ./bin/* ./%var_outdir%/bin/
cp -fr ./lib/* ./%var_outdir%/lib

mv -f bin bin-debug
mv -f lib lib-debug

:rem rm -fr ./bin
:rem rm -fr ./lib

msbuild source/allinone/allinone.sln /m:12 /p:Configuration=Debug /p:Platform=Win32 /t:clean


msbuild source/allinone/allinone.sln /m:12 /p:Configuration=Release /p:Platform=Win32

cp -f ./bin/*.* ./%var_outdir%/bin/
cp -f ./lib/*.lib ./%var_outdir%/lib/

mv -f bin bin-release
mv -f lib lib-release

:rem rm -fr ./bin
:rem rm -fr ./lib

msbuild source/allinone/allinone.sln /m:12 /p:Configuration=Release /p:Platform=Win32 /t:clean

touch ./%var_outdir%/%var_outdir%.txt

