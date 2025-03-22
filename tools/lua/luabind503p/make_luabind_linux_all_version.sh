#!/bin/sh

sh make_luabind_linux_tpl.sh 503 501
sh make_luabind_linux_tpl.sh 503 502
sh make_luabind_linux_tpl.sh 503 503
sh make_luabind_linux_tpl.sh 503 jit501
sh rm_luabind_linux_sample.sh