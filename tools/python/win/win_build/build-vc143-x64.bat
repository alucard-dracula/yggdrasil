
setlocal

call "d:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

cd /d %~dp0

set stage_dir=stage

mkdir "%stage_dir%"

:release
rm -fr "./amd64"
rm -fr "./obj"
rm -fr "%stage_dir%/python-vc143-x64-release"

call build.bat -r -p x64 -c Release

cp -fr "./amd64" "%stage_dir%/python-vc143-x64-release"

:debug
rm -fr "./amd64"
rm -fr "./obj"
rm -fr "%stage_dir%/python-vc143-x64-debug"

call build.bat -d -r -p x64 -c Debug

cp -fr "./amd64" "%stage_dir%/python-vc143-x64-debug"

endlocal