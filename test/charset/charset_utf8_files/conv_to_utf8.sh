#!/bin/sh

#find ./ -type f -name '*.cpp'

rm -fr ./out
mkdir out

find . -type f -name '*.cpp' -exec sh -c "iconv -f GBK -t utf-8 {} > out/{}" \;

