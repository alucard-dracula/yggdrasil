cd /d %~dp0

set mid_dir=./Win32/Debug
set out_dir=./stage

mkdir "%out_dir%/bin-debug"
mkdir "%out_dir%/lib-debug"

msbuild cyrus-sasl-all-in-one.sln /t:rebuild /p:Platform=x86;Configuration=Debug

cp -f %mid_dir%/*.lib %out_dir%/lib-debug/
cp -f %mid_dir%/*.dll %out_dir%/bin-debug/

if not exist %out_dir%\include goto copy_include

goto end

:copy_include

mkdir "%out_dir%/include/sasl"
cp -fr ../include/*.h "%out_dir%/include/sasl/"

:end

