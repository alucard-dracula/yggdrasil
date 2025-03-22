cd /d %~dp0

b2 -j12 toolset=msvc-10.0 -sZLIB_SOURCE=e:/devel/build_tmp/zlib-1.2.12 -sICU_PATH=d:/libicu --build-type=complete address-model=64 threading=multi

