
cd /d %~dp0

set var_root_dir=%~dp0

set var_python_ver=313
set var_python_ver_main=%var_python_ver:~0,1%

echo "%var_python_ver_main%"

set python3_dll=python%var_python_ver_main%.dll
set python3_def=%python3_dll%.def
set python3_a=libpython%var_python_ver_main%.a

set python31x_dll=python%var_python_ver%.dll
set python31x_def=%python31x_dll%.def
set python31x_a=libpython%var_python_ver%.a

echo "%python3_dll%"
echo "%python3_def%"
echo "%python3_a%"

echo "%python31x_dll%"
echo "%python31x_def%"
echo "%python31x_a%"

echo "%var_root_dir%libs\%python31x_a%"


pexports %python3_dll% > %python3_def%
dlltool -d %python3_def% -D %python3_dll% -l %var_root_dir%libs\%python3_a%

pexports %python31x_dll% > %python31x_def%
dlltool -d %python31x_def% -D %python31x_dll% -l %var_root_dir%libs\%python31x_a%
