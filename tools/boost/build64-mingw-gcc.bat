cd /d %~dp0
set PATH=%mingw64_path%;%PATH%
b2 -j12 toolset=gcc -sZLIB_SOURCE=e:/devel/build_tmp/zlib-1.3 -sICU_PATH=d:/libicu_mgw14_x64 -sICU_ICUUC_NAME=icuuc.dll -sICU_ICUDT_NAME=icudt.dll -sICU_ICUIN_NAME=icuin.dll --build-type=complete address-model=64

