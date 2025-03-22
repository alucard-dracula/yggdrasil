#!/bin/sh

ln -s libdll_cal_test.so dll_cal_test.so
ln -s liblua_calculator.so lua_calculator.so
ln -s liblua_cal_svr_procotol.so lua_cal_svr_procotol.so
ln -s liblua_hello_dll.so lua_hello_dll.so
ln -s libyggr_lua_base_type.so yggr_lua_base_type.so

chmod a+x *
chmod a-x *.lua

dos2unix begin_test_env_android_termux.sh
dos2unix end_test_env_termux.sh

