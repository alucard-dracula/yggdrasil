#!/bin/sh

luaver_old=lua$1
luaver_new=lua$2

echo ${luaver_old}
echo ${luaver_new}

cp -f ./include/include_linux.cbp ./include/include_linux_${luaver_new}.cbp 
cp -f ./include/include_linux_cpp03.cbp ./include/include_linux_${luaver_new}_cpp03.cbp 
cp -f ./include/include_linux_cpp11.cbp ./include/include_linux_${luaver_new}_cpp11.cbp 
cp -f ./include/include_linux_cpp14.cbp ./include/include_linux_${luaver_new}_cpp14.cbp 
cp -f ./include/include_linux_cpp17.cbp ./include/include_linux_${luaver_new}_cpp17.cbp 
cp -f ./include/include_linux_cpp20.cbp ./include/include_linux_${luaver_new}_cpp20.cbp 

cat ./luabind/luabind_linux.cbp | sed "s/-l${luaver_old}/-l${luaver_new}/g" > ./luabind/luabind_linux_${luaver_new}.cbp
cat ./luabind/luabind_linux_cpp03.cbp | sed "s/-l${luaver_old}/-l${luaver_new}/g" > ./luabind/luabind_linux_${luaver_new}_cpp03.cbp
cat ./luabind/luabind_linux_cpp11.cbp | sed "s/-l${luaver_old}/-l${luaver_new}/g" > ./luabind/luabind_linux_${luaver_new}_cpp11.cbp
cat ./luabind/luabind_linux_cpp14.cbp | sed "s/-l${luaver_old}/-l${luaver_new}/g" > ./luabind/luabind_linux_${luaver_new}_cpp14.cbp
cat ./luabind/luabind_linux_cpp17.cbp | sed "s/-l${luaver_old}/-l${luaver_new}/g" > ./luabind/luabind_linux_${luaver_new}_cpp17.cbp
cat ./luabind/luabind_linux_cpp20.cbp | sed "s/-l${luaver_old}/-l${luaver_new}/g" > ./luabind/luabind_linux_${luaver_new}_cpp20.cbp

cp -f ./luabind_static/luabind_static_linux.cbp ./luabind_static/luabind_static_linux_${luaver_new}.cbp 
cp -f ./luabind_static/luabind_static_linux_cpp03.cbp ./luabind_static/luabind_static_linux_${luaver_new}_cpp03.cbp 
cp -f ./luabind_static/luabind_static_linux_cpp11.cbp ./luabind_static/luabind_static_linux_${luaver_new}_cpp11.cbp 
cp -f ./luabind_static/luabind_static_linux_cpp14.cbp ./luabind_static/luabind_static_linux_${luaver_new}_cpp14.cbp 
cp -f ./luabind_static/luabind_static_linux_cpp17.cbp ./luabind_static/luabind_static_linux_${luaver_new}_cpp17.cbp 
cp -f ./luabind_static/luabind_static_linux_cpp20.cbp ./luabind_static/luabind_static_linux_${luaver_new}_cpp20.cbp 

cp -f ./luabind_static_ts/luabind_static_ts_linux.cbp ./luabind_static_ts/luabind_static_ts_linux_${luaver_new}.cbp 
cp -f ./luabind_static_ts/luabind_static_ts_linux_cpp03.cbp ./luabind_static_ts/luabind_static_ts_linux_${luaver_new}_cpp03.cbp 
cp -f ./luabind_static_ts/luabind_static_ts_linux_cpp11.cbp ./luabind_static_ts/luabind_static_ts_linux_${luaver_new}_cpp11.cbp 
cp -f ./luabind_static_ts/luabind_static_ts_linux_cpp14.cbp ./luabind_static_ts/luabind_static_ts_linux_${luaver_new}_cpp14.cbp 
cp -f ./luabind_static_ts/luabind_static_ts_linux_cpp17.cbp ./luabind_static_ts/luabind_static_ts_linux_${luaver_new}_cpp17.cbp 
cp -f ./luabind_static_ts/luabind_static_ts_linux_cpp20.cbp ./luabind_static_ts/luabind_static_ts_linux_${luaver_new}_cpp20.cbp 

cat ./luabind_test/luabind_test_linux.cbp | sed "s/-l${luaver_old}/-l${luaver_new}/g" > ./luabind_test/luabind_test_linux_${luaver_new}.cbp
cat ./luabind_test/luabind_test_linux_cpp03.cbp | sed "s/-l${luaver_old}/-l${luaver_new}/g" > ./luabind_test/luabind_test_linux_${luaver_new}_cpp03.cbp
cat ./luabind_test/luabind_test_linux_cpp11.cbp | sed "s/-l${luaver_old}/-l${luaver_new}/g" > ./luabind_test/luabind_test_linux_${luaver_new}_cpp11.cbp
cat ./luabind_test/luabind_test_linux_cpp14.cbp | sed "s/-l${luaver_old}/-l${luaver_new}/g" > ./luabind_test/luabind_test_linux_${luaver_new}_cpp14.cbp
cat ./luabind_test/luabind_test_linux_cpp17.cbp | sed "s/-l${luaver_old}/-l${luaver_new}/g" > ./luabind_test/luabind_test_linux_${luaver_new}_cpp17.cbp
cat ./luabind_test/luabind_test_linux_cpp20.cbp | sed "s/-l${luaver_old}/-l${luaver_new}/g" > ./luabind_test/luabind_test_linux_${luaver_new}_cpp20.cbp

cat ./luabind_linux.workspace | sed "s/_linux/_linux_${luaver_new}/g" > ./luabind_linux_${luaver_new}.workspace
cat ./luabind_linux_cpp03.workspace | sed "s/_linux/_linux_${luaver_new}/g" > ./luabind_linux_${luaver_new}_cpp03.workspace
cat ./luabind_linux_cpp11.workspace | sed "s/_linux/_linux_${luaver_new}/g" > ./luabind_linux_${luaver_new}_cpp11.workspace
cat ./luabind_linux_cpp14.workspace | sed "s/_linux/_linux_${luaver_new}/g" > ./luabind_linux_${luaver_new}_cpp14.workspace
cat ./luabind_linux_cpp17.workspace | sed "s/_linux/_linux_${luaver_new}/g" > ./luabind_linux_${luaver_new}_cpp17.workspace
cat ./luabind_linux_cpp20.workspace | sed "s/_linux/_linux_${luaver_new}/g" > ./luabind_linux_${luaver_new}_cpp20.workspace

