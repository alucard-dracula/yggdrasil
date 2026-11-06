#!/bin/bash

find ./ -type d -name 'discard' -exec rm -fr {} \;

rm -fr ./test/dont_use
rm -fr ./test/libtorrent_test
rm -fr ./test/tmp_test
rm -fr ./test/tmp_tools

rm -fr ./tools/lua/perbuild
rm -fr ./tools/python/failed

rm -f ./new_features_under_consideration.txt
rm -f ./now_mission.txt



