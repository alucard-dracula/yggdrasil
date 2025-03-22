cd /d %~dp0

rem call "D:/Program Files/Microsoft Visual Studio/2022/Enterprise/VC/Auxiliary/Build/vcvars32.bat"
b2 -j12 toolset=msvc-14.3 -sZLIB_SOURCE=e:/devel/build_tmp/zlib-1.3 -sICU_PATH=d:/libicu --build-type=complete address-model=32 threading=multi

