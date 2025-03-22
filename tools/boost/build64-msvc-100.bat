cd /d %~dp0

b2 -j12 toolset=msvc-10.0 -sZLIB_SOURCE=e:/devel/build_tmp/zlib-1.3 -sICU_PATH=d:/libicu -sICU_ICUUC_NAME=icuuc-vc100 -sICU_ICUDT_NAME=icudt-vc100 -sICU_ICUIN_NAME=icuin-vc100 --build-type=complete address-model=64

