

call "D:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86

cd /d %~dp0

set var_dir=%~dp0
set ctrl_var=%1

call vc_build_tpl.bat VC-WIN32 %var_dir%stage/vc100-x32 %var_dir%stage/vc100-x32/ssl
call vc_build_tpl.bat VC-WIN32 %var_dir%stage/vc100-x32-static %var_dir%stage/vc100-x32-static/ssl -static

if not "%ctrl_var%" == "exit" goto no_quit

exit

:no_quit