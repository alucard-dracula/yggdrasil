

if "%~7"=="has_env" goto :start_mark
setlocal
call "d:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

:start_mark

cd /d %~dp0

set var_dll_dir_path=%~1
set var_dll_name=%~2
set var_dll_path=%var_dll_dir_path%/%var_dll_name%

set var_impl_dir_path=%~3
set var_impl_lib_name=%~4

set var_impl_lib_path=%var_impl_dir_path%/%var_impl_lib_name%

set var_dll_name_arg=%~5
set var_platfrom=%~6

:rem echo "%var_dll_path%"
:rem echo "%var_impl_lib_path%"
:rem echo "%var_dll_name_arg%"
:rem echo "%var_platfrom%"
:rem echo "%~dp0"

set var_def_path=%var_dll_path%.def

if "%var_platfrom%"=="i386" goto :using_tool_chain_dlltool
if "%var_platfrom%"=="x86-64" goto :using_tool_chain_dlltool

set var_dbe_path=%var_dll_path%.dbe

dumpbin /exports %var_dll_path% > %var_dbe_path%
tool_dumpbin_exports_to_def.exe %var_dbe_path% %var_def_path% %var_dll_name_arg%
lib /def:%var_def_path% /machine:%var_platfrom% /out:%var_impl_lib_path%

rm -f %var_dbe_path%

goto :gen_lib_end

:using_tool_chain_dlltool

set var_root_dir=%~dp0

pexports %var_dll_path% > %var_def_path%
sed -i "s/LIBRARY .*/LIBRARY %var_dll_name%/g" %var_def_path%

:rem echo "%var_root_dir%%var_dll_path%"
:rem echo "%var_dll_name%.def"
:rem echo "%var_dll_name%.dll"
:rem echo "%var_root_dir%%var_impl_lib_path%"

cd /d %var_root_dir%%var_dll_dir_path%

dlltool -d %var_dll_name%.def -D %var_dll_name%.dll -l %var_root_dir%%var_impl_lib_path%

cd /d %var_root_dir%

:gen_lib_end

if "%~7"=="has_env" goto :end

endlocal

:end

:debug_end


