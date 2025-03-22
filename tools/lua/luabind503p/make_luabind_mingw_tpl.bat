
cd /d %~dp0

set luaver_old=lua%1
set luaver_new=lua%2

echo %luaver_old%
echo %luaver_new%

cp -f ./include/include_mingw.cbp ./include/include_mingw_%luaver_new%.cbp 
cp -f ./include/include_mingw_cpp03.cbp ./include/include_mingw_%luaver_new%_cpp03.cbp 
cp -f ./include/include_mingw_cpp11.cbp ./include/include_mingw_%luaver_new%_cpp11.cbp 
cp -f ./include/include_mingw_cpp14.cbp ./include/include_mingw_%luaver_new%_cpp14.cbp 
cp -f ./include/include_mingw_cpp17.cbp ./include/include_mingw_%luaver_new%_cpp17.cbp 
cp -f ./include/include_mingw_cpp20.cbp ./include/include_mingw_%luaver_new%_cpp20.cbp 

cat ./luabind/luabind_mingw.cbp | sed 's/-l%luaver_old%/-l%luaver_new%/g' > ./luabind/luabind_mingw_%luaver_new%.cbp
cat ./luabind/luabind_mingw_cpp03.cbp | sed 's/-l%luaver_old%/-l%luaver_new%/g' > ./luabind/luabind_mingw_%luaver_new%_cpp03.cbp
cat ./luabind/luabind_mingw_cpp11.cbp | sed 's/-l%luaver_old%/-l%luaver_new%/g' > ./luabind/luabind_mingw_%luaver_new%_cpp11.cbp
cat ./luabind/luabind_mingw_cpp14.cbp | sed 's/-l%luaver_old%/-l%luaver_new%/g' > ./luabind/luabind_mingw_%luaver_new%_cpp14.cbp
cat ./luabind/luabind_mingw_cpp17.cbp | sed 's/-l%luaver_old%/-l%luaver_new%/g' > ./luabind/luabind_mingw_%luaver_new%_cpp17.cbp
cat ./luabind/luabind_mingw_cpp20.cbp | sed 's/-l%luaver_old%/-l%luaver_new%/g' > ./luabind/luabind_mingw_%luaver_new%_cpp20.cbp

cp -f ./luabind_static/luabind_static_mingw.cbp ./luabind_static/luabind_static_mingw_%luaver_new%.cbp 
cp -f ./luabind_static/luabind_static_mingw_cpp03.cbp ./luabind_static/luabind_static_mingw_%luaver_new%_cpp03.cbp 
cp -f ./luabind_static/luabind_static_mingw_cpp11.cbp ./luabind_static/luabind_static_mingw_%luaver_new%_cpp11.cbp 
cp -f ./luabind_static/luabind_static_mingw_cpp14.cbp ./luabind_static/luabind_static_mingw_%luaver_new%_cpp14.cbp 
cp -f ./luabind_static/luabind_static_mingw_cpp17.cbp ./luabind_static/luabind_static_mingw_%luaver_new%_cpp17.cbp 
cp -f ./luabind_static/luabind_static_mingw_cpp20.cbp ./luabind_static/luabind_static_mingw_%luaver_new%_cpp20.cbp 

cp -f ./luabind_static_ts/luabind_static_ts_mingw.cbp ./luabind_static_ts/luabind_static_ts_mingw_%luaver_new%.cbp 
cp -f ./luabind_static_ts/luabind_static_ts_mingw_cpp03.cbp ./luabind_static_ts/luabind_static_ts_mingw_%luaver_new%_cpp03.cbp 
cp -f ./luabind_static_ts/luabind_static_ts_mingw_cpp11.cbp ./luabind_static_ts/luabind_static_ts_mingw_%luaver_new%_cpp11.cbp 
cp -f ./luabind_static_ts/luabind_static_ts_mingw_cpp14.cbp ./luabind_static_ts/luabind_static_ts_mingw_%luaver_new%_cpp14.cbp 
cp -f ./luabind_static_ts/luabind_static_ts_mingw_cpp17.cbp ./luabind_static_ts/luabind_static_ts_mingw_%luaver_new%_cpp17.cbp 
cp -f ./luabind_static_ts/luabind_static_ts_mingw_cpp20.cbp ./luabind_static_ts/luabind_static_ts_mingw_%luaver_new%_cpp20.cbp 

cat ./luabind_test/luabind_test_mingw.cbp | sed 's/-l%luaver_old%/-l%luaver_new%/g' > ./luabind_test/luabind_test_mingw_%luaver_new%.cbp
cat ./luabind_test/luabind_test_mingw_cpp03.cbp | sed 's/-l%luaver_old%/-l%luaver_new%/g' > ./luabind_test/luabind_test_mingw_%luaver_new%_cpp03.cbp
cat ./luabind_test/luabind_test_mingw_cpp11.cbp | sed 's/-l%luaver_old%/-l%luaver_new%/g' > ./luabind_test/luabind_test_mingw_%luaver_new%_cpp11.cbp
cat ./luabind_test/luabind_test_mingw_cpp14.cbp | sed 's/-l%luaver_old%/-l%luaver_new%/g' > ./luabind_test/luabind_test_mingw_%luaver_new%_cpp14.cbp
cat ./luabind_test/luabind_test_mingw_cpp17.cbp | sed 's/-l%luaver_old%/-l%luaver_new%/g' > ./luabind_test/luabind_test_mingw_%luaver_new%_cpp17.cbp
cat ./luabind_test/luabind_test_mingw_cpp20.cbp | sed 's/-l%luaver_old%/-l%luaver_new%/g' > ./luabind_test/luabind_test_mingw_%luaver_new%_cpp20.cbp

cat ./luabind_mingw.workspace | sed 's/_mingw/_mingw_%luaver_new%/g' > ./luabind_mingw_%luaver_new%.workspace
cat ./luabind_mingw_cpp03.workspace | sed 's/_mingw/_mingw_%luaver_new%/g' > ./luabind_mingw_%luaver_new%_cpp03.workspace
cat ./luabind_mingw_cpp11.workspace | sed 's/_mingw/_mingw_%luaver_new%/g' > ./luabind_mingw_%luaver_new%_cpp11.workspace
cat ./luabind_mingw_cpp14.workspace | sed 's/_mingw/_mingw_%luaver_new%/g' > ./luabind_mingw_%luaver_new%_cpp14.workspace
cat ./luabind_mingw_cpp17.workspace | sed 's/_mingw/_mingw_%luaver_new%/g' > ./luabind_mingw_%luaver_new%_cpp17.workspace
cat ./luabind_mingw_cpp20.workspace | sed 's/_mingw/_mingw_%luaver_new%/g' > ./luabind_mingw_%luaver_new%_cpp20.workspace

