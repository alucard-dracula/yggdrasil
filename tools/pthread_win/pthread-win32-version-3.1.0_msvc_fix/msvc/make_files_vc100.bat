
:rem make_file ms_version (e.g make_file 100)
@echo off
cd /d %~dp0

setlocal
call "d:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

call make_files_tpl.bat -vc100
rm -f std_files_using_vc143.txt
touch std_files_using_vc100.txt

endlocal

