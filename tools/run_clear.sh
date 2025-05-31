#!/bin/sh

if [ -d "./cyrus-sasl2/krb5-1.21.3-final/src/not-remove-lib" ]; then
	mv -f ./cyrus-sasl2/krb5-1.21.3-final/src/not-remove-lib ./cyrus-sasl2/krb5-1.21.3-final/src/lib
fi

find ./ -type d -name 'Debug' -exec rm -fr {} \;
find ./ -type d -name 'Release' -exec rm -fr {} \;
find ./ -type d -name 'Debug64' -exec rm -fr {} \;
find ./ -type d -name 'Release64' -exec rm -fr {} \;
find ./ -type d -name 'Debug-Win32' -exec rm -fr {} \;
find ./ -type d -name 'Debug-x32' -exec rm -fr {} \;
find ./ -type d -name 'Debug-x64' -exec rm -fr {} \;
find ./ -type d -name 'Debug-a32' -exec rm -fr {} \;
find ./ -type d -name 'Debug-a64' -exec rm -fr {} \;
find ./ -type d -name 'Release-Win32' -exec rm -fr {} \;
find ./ -type d -name 'Release-x32' -exec rm -fr {} \;
find ./ -type d -name 'Release-x64' -exec rm -fr {} \;
find ./ -type d -name 'Release-a32' -exec rm -fr {} \;
find ./ -type d -name 'Release-a64' -exec rm -fr {} \;
find ./ -type d -name '.vscode' -exec rm -fr {} \;
find ./ -type d -name '.vs' -exec rm -fr {} \;

if [ -d "./cyrus-sasl2/krb5-1.21.3-final/src/lib" ]; then
	mv -f ./cyrus-sasl2/krb5-1.21.3-final/src/lib ./cyrus-sasl2/krb5-1.21.3-final/src/not-remove-lib
fi

find ./ -type d -name 'lib' -exec rm -fr {} \;

if [ -d "./cyrus-sasl2/krb5-1.21.3-final/src/not-remove-lib" ]; then
	mv -f ./cyrus-sasl2/krb5-1.21.3-final/src/not-remove-lib ./cyrus-sasl2/krb5-1.21.3-final/src/lib
fi

find ./ -type d -name 'lib64' -exec rm -fr {} \;
find ./ -type d -name 'bin' -exec rm -fr {} \;
find ./ -type d -name 'bin64' -exec rm -fr {} \;
find ./ -type d -name 'exe32' -exec rm -fr {} \;
find ./ -type d -name 'exe64' -exec rm -fr {} \;
find ./ -type d -name 'stage_prefix' -exec rm -fr {} \;

find ./ -type d -name 'obj' -exec rm -fr {} \;
find ./ -type d -name 'stage' -exec rm -fr {} \;

find ./ -type d -name 'stage_lua501' -exec rm -fr {} \;
find ./ -type d -name 'stage_lua502' -exec rm -fr {} \;
find ./ -type d -name 'stage_lua503' -exec rm -fr {} \;
find ./ -type d -name 'stage_luajit21' -exec rm -fr {} \;

find ./ -type d -name 'cmake_build_cache*' -exec rm -fr {} \;

find ./ -type f -name '*.sdf' -exec rm -f {} \;
find ./ -type f -name '*.suo' -exec rm -f {} \;
find ./ -type f -name '*.user' -exec rm -f {} \;
find ./ -type f -name '*.depend' -exec rm -f {} \;
find ./ -type f -name '*.layout' -exec rm -f {} \;

find ./ -type f -name '.DS_Store' -exec rm -f {} \;

find ./ -type f -name '*.exe' -exec rm -f {} \;
find ./ -type f -name 'tool_*_log.txt' -exec rm -f {} \;
