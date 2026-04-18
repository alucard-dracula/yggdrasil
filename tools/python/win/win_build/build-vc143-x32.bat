
setlocal

call "d:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars32.bat"

cd /d %~dp0

set stage_dir=stage

mkdir "%stage_dir%"

:release
rm -fr "./win32"
rm -fr "./obj"
rm -fr "%stage_dir%/python-vc143-x32-release"

call build.bat -r -p Win32 -c Release

cp -fr "./win32" "%stage_dir%/python-vc143-x32-release"

:debug
rm -fr "./win32"
rm -fr "./obj"
rm -fr "%stage_dir%/python-vc143-x32-debug"

call build.bat -d -r -p Win32 -c Debug

cp -fr "./win32" "%stage_dir%/python-vc143-x32-debug"

endlocal