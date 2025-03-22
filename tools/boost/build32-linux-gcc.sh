#!/bin/sh

# errors.user-error : "$(ICU_ICUUC_NAME)" ;

./b2 \
    -j12 \
    toolset=gcc \
    -sZLIB_SOURCE=/home/alucard/yggdrasil_workspace_linux/build_tmp/zlib-1.3 \
    -sICU_PATH=/home/alucard/third_part/libicu58_gcc14_x32 \
    -sICU_ICUUC_NAME=icuuc \
    -sICU_ICUDT_NAME=icudata \
    -sICU_ICUIN_NAME=icui18n \
    --prefix=/home/alucard/third_part/boost_182_x32 \
    --build-type=complete \
    --layout=versioned \
    address-model=32 \
    install