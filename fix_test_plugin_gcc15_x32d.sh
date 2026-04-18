#!/bin/bash

rm -f Debug-x32/gcc15/yggr_lua_base_type.so
ln -s libyggr_lua_base_type.so Debug-x32/gcc15/yggr_lua_base_type.so

#rm -f Debug-x32/gcc15/mongo_crypt_v1.so
#ln -s /usr/local/lib/mongo_crypt/mongo_crypt_v1.so  Debug-x32/gcc15/mongo_crypt_v1.so

rm -f Debug-x32/gcc15/pyst.so
ln -s libpyst.pyd Debug-x32/gcc15/pyst.so
