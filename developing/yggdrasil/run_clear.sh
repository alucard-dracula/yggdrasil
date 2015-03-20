#!/bin/sh
find ./ -type d -name 'Debug*' -exec rm -fr {} \;
find ./ -type d -name 'Release*' -exec rm -fr {} \;
rm -fr ./lib64
rm -fr ./lib
