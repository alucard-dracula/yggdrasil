#!/bin/sh

var_clang_tag="clang-darwin$(clang --version | awk '/version/ {print $4}' | cut -d. -f1)"

rm -f test/yggr_lua_base_type.so
rm -f Release-x64/${var_clang_tag}/yggr_lua_base_type.so
rm -f lib/Release-x64/yggr_lua_base_type.dylib
ln -s lib/Release-x64/libyggr_lua_base_type.dylib Release-x64/${var_clang_tag}/yggr_lua_base_type.dylib
ln -s ../../lib/Release-x64/libyggr_lua_base_type.dylib Release-x64/${var_clang_tag}/yggr_lua_base_type.so
ln -s ../lib/Release-x64/libyggr_lua_base_type.dylib test/yggr_lua_base_type.so