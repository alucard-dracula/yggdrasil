#!/bin/sh

target_dir=./Release-x64/gcc14

cp -f ./lua_hello_dll/test_lua_hello_dll.lua ${target_dir}/
cp -f ./lua_active_svr_test/cal_num_test.lua ${target_dir}/
cp -f ./lua_clt_test/create_cdt.lua ${target_dir}/

rm -f ${target_dir}/dll_cal_test.so
rm -f ${target_dir}/lua_calculator.so
rm -f ${target_dir}/lua_cal_svr_procotol.so
rm -f ${target_dir}/lua_hello_dll.so
rm -f ${target_dir}/yggr_lua_base_type.so

ln -s libdll_cal_test.so ${target_dir}/dll_cal_test.so
ln -s liblua_calculator.so ${target_dir}/lua_calculator.so
ln -s liblua_cal_svr_procotol.so ${target_dir}/lua_cal_svr_procotol.so
ln -s liblua_hello_dll.so ${target_dir}/lua_hello_dll.so
ln -s libyggr_lua_base_type.so ${target_dir}/yggr_lua_base_type.so
