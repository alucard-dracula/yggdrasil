#!/bin/sh

var_prefix_dir=$1
var_stage_dir=$2
var_sdk_name=$3
var_sdk_ver_min=$4
var_icu_root_path=$5
var_zlib_source_path=$6

echo "${var_prefix_dir}"
echo "${var_stage_dir}"
echo "${var_sdk_name}"
echo "${var_sdk_ver_min}"
echo "${var_icu_root_path}"
echo "${var_zlib_source_path}"

if [ "${var_sdk_name}" = "macosx" ]; then
    var_sdk_flag="-mmacosx-version-min=${var_sdk_ver_min}"
elif [ "${var_sdk_name}" = "iphoneos" ]; then
    var_sdk_flag="-miphoneos-version-min=${var_sdk_ver_min}"
else
    var_sdk_flag=""
fi

if [ "${var_sdk_name}" = "iphoneos" ]; then
    var_sdk_arch=arm
    #var_sdk_arch_flag="-arch arm64"
else
    var_sdk_arch=arm+x86
    #var_sdk_arch_flag="-arch x86_64 -arch arm64"
fi

var_sdk_root=$(xcrun --sdk ${var_sdk_name} --show-sdk-path)

if [ "${USRDEF_FORCE_HAS_ICU}" = "yes" ]; then
    var_forece_has_icu="-DBOOST_HAS_ICU=1"
else
    var_forece_has_icu=""
fi

var_cc=clang
var_cflags="-fPIC -isysroot ${var_sdk_root} ${var_sdk_flag} ${var_forece_has_icu}"
var_cxxflags="${var_cflags}"

echo "${var_cc}"
echo "${var_cflags}"
echo "${var_cxxflags}"

if [ "${var_sdk_name}" = "macosx" ]; then
    ./b2 \
        --without-mpi \
        --without-graph_parallel \
        --without-fiber \
        --without-coroutine \
        --without-math \
        -j12 \
        toolset=${var_cc} \
        -sICU_PATH=${var_icu_root_path} \
        -sICU_ICUUC_NAME=icuuc \
        -sICU_ICUDT_NAME=icudata \
        -sICU_ICUIN_NAME=icui18n \
        architecture=${var_sdk_arch} \
        cxxstd=11 \
        cxxflags="${var_cxxflags}" \
        address-model=64 \
        --build-type=complete \
        --layout=versioned \
        --stagedir=${var_stage_dir}

    ./b2 \
        --with-fiber \
        --with-coroutine \
        --with-math \
        -j12 \
        toolset=${var_cc} \
        -sICU_PATH=${var_icu_root_path} \
        -sICU_ICUUC_NAME=icuuc \
        -sICU_ICUDT_NAME=icudata \
        -sICU_ICUIN_NAME=icui18n \
        architecture=x86 \
        cxxstd=11 \
        cxxflags="${var_cxxflags}" \
        address-model=64 \
        --build-type=complete \
        --layout=versioned \
        --stagedir=${var_stage_dir}/x86_64_only 

    ./b2 \
        --with-fiber \
        --with-coroutine \
        --with-math \
        -j12 \
        toolset=${var_cc} \
        -sICU_PATH=${var_icu_root_path} \
        -sICU_ICUUC_NAME=icuuc \
        -sICU_ICUDT_NAME=icudata \
        -sICU_ICUIN_NAME=icui18n \
        architecture=arm \
        abi=aapcs \
        cxxstd=11 \
        cxxflags="${var_cxxflags}" \
        address-model=64 \
        --build-type=complete \
        --layout=versioned \
        --stagedir=${var_stage_dir}/arm64_only 

    sh build64-boost-darwin16_merge.sh "${var_stage_dir}/arm64_only/lib" "${var_stage_dir}/x86_64_only/lib" "${var_stage_dir}/fat_only"
    find "${var_stage_dir}/fat_only" -type f -exec cp -f {} "${var_stage_dir}/lib/" \;


elif [ "${var_sdk_name}" = "iphoneos" ]; then

    ./b2 \
        --without-mpi \
        --without-graph_parallel \
        --without-python \
        -j12 \
        toolset=${var_cc} \
        -sICU_PATH=${var_icu_root_path} \
        -sICU_ICUUC_NAME=icuuc \
        -sICU_ICUDT_NAME=icudata \
        -sICU_ICUIN_NAME=icui18n \
        architecture=${var_sdk_arch} \
        cxxstd=11 \
        cxxflags="${var_cxxflags}" \
        address-model=64 \
        --build-type=complete \
        --layout=versioned \
        --stagedir=${var_stage_dir} \
        link=static \
        runtime-link=static

else

    ./b2 \
        --without-mpi \
        --without-graph_parallel \
        --without-fiber \
        --without-coroutine \
        --without-math \
        --without-python \
        -j12 \
        toolset=${var_cc} \
        -sICU_PATH=${var_icu_root_path} \
        -sICU_ICUUC_NAME=icuuc \
        -sICU_ICUDT_NAME=icudata \
        -sICU_ICUIN_NAME=icui18n \
        architecture=${var_sdk_arch} \
        cxxstd=11 \
        cxxflags="${var_cxxflags}" \
        address-model=64 \
        --build-type=complete \
        --layout=versioned \
        --stagedir=${var_stage_dir} \
        link=static \
        runtime-link=static

    ./b2 \
        --with-fiber \
        --with-coroutine \
        --with-math \
        -j12 \
        toolset=${var_cc} \
        -sICU_PATH=${var_icu_root_path} \
        -sICU_ICUUC_NAME=icuuc \
        -sICU_ICUDT_NAME=icudata \
        -sICU_ICUIN_NAME=icui18n \
        architecture=x86 \
        cxxstd=11 \
        cxxflags="${var_cxxflags}" \
        address-model=64 \
        --build-type=complete \
        --layout=versioned \
        --stagedir=${var_stage_dir}/x86_64_only \
        link=static \
        runtime-link=static

    ./b2 \
        --with-fiber \
        --with-coroutine \
        --with-math \
        -j12 \
        toolset=${var_cc} \
        -sICU_PATH=${var_icu_root_path} \
        -sICU_ICUUC_NAME=icuuc \
        -sICU_ICUDT_NAME=icudata \
        -sICU_ICUIN_NAME=icui18n \
        architecture=arm \
        abi=aapcs \
        cxxstd=11 \
        cxxflags="${var_cxxflags}" \
        address-model=64 \
        --build-type=complete \
        --layout=versioned \
        --stagedir=${var_stage_dir}/arm64_only \
        link=static \
        runtime-link=static

    sh build64-boost-darwin16_merge.sh "${var_stage_dir}/arm64_only/lib" "${var_stage_dir}/x86_64_only/lib" "${var_stage_dir}/fat_only"
    find "${var_stage_dir}/fat_only" -type f -exec cp -f {} "${var_stage_dir}/lib/" \;
fi
