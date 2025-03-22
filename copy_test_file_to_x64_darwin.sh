#!/bin/sh

target_dir_exe=./Release-x64/clang-darwin16
target_dir_lib=./lib/Release-x64

cp -f ./lua_hello_dll/test_lua_hello_dll.lua ${target_dir_exe}/
cp -f ./lua_active_svr_test/cal_num_test.lua ${target_dir_exe}/
cp -f ./lua_clt_test/create_cdt.lua ${target_dir_exe}/

rm -f ${target_dir_exe}/lua_calculator.so
rm -f ${target_dir_exe}/lua_cal_svr_procotol.so
rm -f ${target_dir_exe}/lua_hello_dll.so

rm -f ${target_dir_lib}/dll_cal_test.dylib
rm -f ${target_dir_lib}/lua_calculator.dylib
rm -f ${target_dir_lib}/lua_cal_svr_procotol.dylib
rm -f ${target_dir_lib}/lua_hello_dll.dylib
rm -f ${target_dir_lib}/yggr_lua_base_type.dylib

ln -s ../../lib/Release-x64/liblua_calculator.dylib ${target_dir_exe}/lua_calculator.so
ln -s ../../lib/Release-x64/liblua_cal_svr_procotol.dylib ${target_dir_exe}/lua_cal_svr_procotol.so
ln -s ../../lib/Release-x64/liblua_hello_dll.dylib ${target_dir_exe}/lua_hello_dll.so

ln -s libdll_cal_test.dylib ${target_dir_lib}/dll_cal_test.dylib
ln -s liblua_calculator.dylib ${target_dir_lib}/lua_calculator.dylib
ln -s liblua_cal_svr_procotol.dylib ${target_dir_lib}/lua_cal_svr_procotol.dylib
ln -s liblua_hello_dll.dylib ${target_dir_lib}/lua_hello_dll.dylib
ln -s libyggr_lua_base_type.dylib ${target_dir_lib}/yggr_lua_base_type.dylib

