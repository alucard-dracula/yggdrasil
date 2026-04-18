#!/bin/bash

rm -f yggr_lua_base_type.so
rm -f pyst.so

chmod a+x *

ln -s libyggr_lua_base_type.so yggr_lua_base_type.so
ln -s libpyst.pyd.so pyst.so

dos2unix begin_test_env_android_termux.sh
dos2unix end_test_env_termux.sh

