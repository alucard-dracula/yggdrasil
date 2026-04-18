#!/bin/sh

var_clang_tag="clang-darwin$(clang --version | awk '/version/ {print $4}' | cut -d. -f1)"

rm -f test/yggr_lua_base_type.so
rm -f Debug-x64/${var_clang_tag}/yggr_lua_base_type.so
rm -f lib/Debug-x64/yggr_lua_base_type.dylib
ln -s lib/Debug-x64/libyggr_lua_base_type.dylib Debug-x64/${var_clang_tag}/yggr_lua_base_type.dylib
ln -s ../../lib/Debug-x64/libyggr_lua_base_type.dylib Debug-x64/${var_clang_tag}/yggr_lua_base_type.so
ln -s ../lib/Debug-x64/libyggr_lua_base_type.dylib test/yggr_lua_base_type.so