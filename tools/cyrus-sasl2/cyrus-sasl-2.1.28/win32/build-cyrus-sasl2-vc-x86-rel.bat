cd /d %~dp0

set mid_dir=./Win32/Release
set out_dir=./stage

mkdir "%out_dir%/bin"
mkdir "%out_dir%/lib"

msbuild cyrus-sasl-all-in-one.sln /t:rebuild /p:Platform=x86;Configuration=Release

cp -f %mid_dir%/*.lib %out_dir%/lib/
cp -f %mid_dir%/*.dll %out_dir%/bin/

if not exist %out_dir%\include goto copy_include

goto end

:copy_include

mkdir "%out_dir%/include/sasl"
cp -fr ../include/*.h "%out_dir%/include/sasl/"

:end