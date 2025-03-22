

call "D:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" amd64

cd /d %~dp0

set var_dir=%~dp0
set ctrl_var=%1

call vc_build_tpl.bat VC-WIN64A %var_dir%stage/vc100-x64 %var_dir%stage/vc100-x64/ssl
call vc_build_tpl.bat VC-WIN64A %var_dir%stage/vc100-x64-static %var_dir%stage/vc100-x64-static/ssl -static

if not "%ctrl_var%" == "exit" goto no_quit

exit

:no_quit