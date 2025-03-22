
cd /d %~dp0

call make_luabind_mingw_tpl.bat 503 501
call make_luabind_mingw_tpl.bat 503 502
call make_luabind_mingw_tpl.bat 503 503
call make_luabind_mingw_tpl.bat 503 jit501
call rm_luabind_mingw_sample.bat