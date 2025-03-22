

call "d:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

cd /d %~dp0

set var_dir=%~dp0
set ctrl_var=%1

call vc_build_tpl.bat VC-WIN64A %var_dir%stage/vc143-x64 %var_dir%stage/vc143-x64/ssl
call vc_build_tpl.bat VC-WIN64A %var_dir%stage/vc143-x64-static %var_dir%stage/vc143-x64-static/ssl -static

if not "%ctrl_var%" == "exit" goto no_quit

exit

:no_quit