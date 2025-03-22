

call "d:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars32.bat"

cd /d %~dp0

set var_dir=%~dp0
set ctrl_var=%1

call vc_build_tpl.bat VC-WIN32 %var_dir%stage/vc143-x32 %var_dir%stage/vc143-x32/ssl
call vc_build_tpl.bat VC-WIN32 %var_dir%stage/vc143-x32-static %var_dir%stage/vc143-x32-static/ssl -static

if not "%ctrl_var%" == "exit" goto no_quit

exit

:no_quit

