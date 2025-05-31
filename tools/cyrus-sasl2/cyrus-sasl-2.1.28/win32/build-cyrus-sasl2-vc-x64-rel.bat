cd /d %~dp0

set mid_dir=./x64/Release
set out_dir=./stage

mkdir "%out_dir%/bin64"
mkdir "%out_dir%/lib64"

msbuild cyrus-sasl-all-in-one.sln /t:rebuild /p:Platform=x64;Configuration=Release

cp -f %mid_dir%/*.lib %out_dir%/lib64/
cp -f %mid_dir%/*.dll %out_dir%/bin64/

if not exist %out_dir%\include goto copy_include

goto end

:copy_include

mkdir "%out_dir%/include/sasl"
cp -fr ../include/*.h "%out_dir%/include/sasl/"

:end