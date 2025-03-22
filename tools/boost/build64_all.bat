
cd /d %~dp0

set prefix_dir_root=boost_last_stage
set prefix_dir_bin=%prefix_dir_root%/bin64
set prefix_dir_lib=%prefix_dir_root%/lib64
set prefix_dir_inc=%prefix_dir_root%/include

rm -fr "%prefix_dir_bin%"
rm -fr "%prefix_dir_lib%"

mkdir "%prefix_dir_bin%"
mkdir "%prefix_dir_lib%"
mkdir "%prefix_dir_inc%"

rm -fr "bin.v2"
rm -fr "stage"

call build64-msvc-100.bat

rm -fr ./stage/cmake
mv -f ./stage/lib/*.lib "%prefix_dir_lib%/"
mv -f ./stage/lib/*.* "%prefix_dir_bin%/"

rm -fr "bin.v2"
rm -fr "stage"

call build64-msvc-143.bat

rm -fr ./stage/cmake
mv -f ./stage/lib/*.lib "%prefix_dir_lib%/"
mv -f ./stage/lib/*.* "%prefix_dir_bin%/"

rm -fr "bin.v2"
rm -fr "stage"

call build64-mingw-gcc.bat

rm -fr ./stage/cmake
mv -f ./stage/lib/*.a "%prefix_dir_lib%/"
mv -f ./stage/lib/*.* "%prefix_dir_bin%/"

rm -fr "bin.v2"
rm -fr "stage"
