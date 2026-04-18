setlocal

call "D:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86

cd /d %~dp0

set var_dir=%~dp0
set ctrl_var=%1

set vc_mark="vc100"
set vc_platform_mark="-x32"

set dir_debug_md="%vc_mark%%vc_platform_mark%-debug"
set dir_debug_static_md="%vc_mark%%vc_platform_mark%-static-debug"

set dir_release_md="%vc_mark%%vc_platform_mark%-release"
set dir_release_static_md="%vc_mark%%vc_platform_mark%-static-release"

set dir_debug_mt="%vc_mark%%vc_platform_mark%-debug-mt"
set dir_debug_static_mt="%vc_mark%%vc_platform_mark%-static-debug-mt"

set dir_release_mt="%vc_mark%%vc_platform_mark%-release-mt"
set dir_release_static_mt="%vc_mark%%vc_platform_mark%-static-release-mt"

call vc_build_tpl.bat debug-VC-WIN32 %var_dir%stage/%dir_debug_md% %var_dir%stage/%dir_debug_md%/ssl "shared"
call vc_build_tpl.bat debug-VC-WIN32 %var_dir%stage/%dir_debug_static_md% %var_dir%stage/%dir_debug_static_md%/ssl "no-shared"

call vc_build_tpl.bat VC-WIN32 %var_dir%stage/%dir_release_md% %var_dir%stage/%dir_release_md%/ssl "shared"
call vc_build_tpl.bat VC-WIN32 %var_dir%stage/%dir_release_static_md% %var_dir%stage/%dir_release_static_md%/ssl "no-shared"


call vc_build_tpl.bat debug-VC-WIN32-HYBRIDCRT-100 %var_dir%stage/%dir_debug_mt% %var_dir%stage/%dir_debug_mt%/ssl "shared"
call vc_build_tpl.bat debug-VC-WIN32-HYBRIDCRT-100 %var_dir%stage/%dir_debug_static_mt% %var_dir%stage/%dir_debug_static_mt%/ssl "no-shared"

call vc_build_tpl.bat VC-WIN32-HYBRIDCRT-100 %var_dir%stage/%dir_release_mt% %var_dir%stage/%dir_release_mt%/ssl "shared"
call vc_build_tpl.bat VC-WIN32-HYBRIDCRT-100 %var_dir%stage/%dir_release_static_mt% %var_dir%stage/%dir_release_static_mt%/ssl "no-shared"

if not "%ctrl_var%" == "exit" goto no_quit

exit

:no_quit

endlocal
