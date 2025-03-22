
cd /d %~dp0

set var_outdir=%1

mkdir "./%var_outdir%/bin64"
mkdir "./%var_outdir%/lib64"

msbuild source/allinone/allinone.sln /m:12 /p:Configuration=Debug /p:Platform=x64

cp -fr ./bin64/* ./%var_outdir%/bin64/
cp -fr ./lib64/* ./%var_outdir%/lib64/

mv -f bin64 bin64-debug
mv -f lib64 lib64-debug

:rem rm -fr ./bin64
:rem rm -fr ./lib64

msbuild source/allinone/allinone.sln /m:12 /p:Configuration=Debug /p:Platform=x64 /t:clean


msbuild source/allinone/allinone.sln /m:12 /p:Configuration=Release /p:Platform=x64

cp -fr ./bin64/* ./%var_outdir%/bin64/
cp -f ./lib64/*.lib ./%var_outdir%/lib64/

mv -f bin64 bin64-release
mv -f lib64 lib64-release

:rem rm -fr ./bin64
:rem rm -fr ./lib64

msbuild source/allinone/allinone.sln /m:12 /p:Configuration=Release /p:Platform=x64 /t:clean

touch ./%var_outdir%/%var_outdir%.txt
