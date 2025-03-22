
cd /d %~dp0

set org_path=%path%
set path=%mingw64_path%;%org_path%

gcc -posix -E -dM - < nul > mingw64_macro.txt

set path=%mingw32_path%;%org_path%

gcc -posix -E -dM - < nul > mingw32_macro.txt

