#!/bin/sh

rm -fr ./data/obj
rm -fr ./data/out

rm -fr bin
rm -fr lib

rm -fr ../bin64-debug
rm -fr ../bin64-release

rm -fr ../lib64-debug
rm -fr ../lib64-release

rm -fr ../bin-debug
rm -fr ../bin-release

rm -fr ../lib-debug
rm -fr ../lib-release

rm -fr ../include

find ./ -type d -name 'x86' -exec rm -fr {} \;
find ./ -type d -name 'x64' -exec rm -fr {} \;
find ./ -type d -name '.vs' -exec rm -fr {} \;

find ./ -type f -name '*.user' -exec rm -f {} \;
find ./ -type f -name '*.sdf' -exec rm -f {} \;
find ./ -type f -name '*.suo' -exec rm -f {} \;

find ./ -type f -name '*.o' -exec rm -f {} \;
find ./ -type f -name '*.ao' -exec rm -f {} \;



