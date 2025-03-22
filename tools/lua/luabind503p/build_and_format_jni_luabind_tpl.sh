#!/bin/sh

var_ndk_root=/d/android_devel/Android/Sdk/ndk/28.0.12674087
var_ndk_build_cmd=${var_ndk_root}/ndk-build.cmd

var_lua_ver=$1
var_cpp_ver=$2

var_android_api=21
var_outdir_root=./stage
var_middir_root=./stage/objs
var_middir_local=${var_middir_root}/local

if [ "${var_lua_ver}" = "501" ]; then
    var_outdir=${var_outdir_root}/luabind_lua501
elif [ "${var_lua_ver}" = "502" ]; then
     var_outdir=${var_outdir_root}/luabind_lua502
elif [ "${var_lua_ver}" = "503" ]; then
     var_outdir=${var_outdir_root}/luabind_lua503
#elif [ "${var_lua_ver}" = "504" ]; then
#     var_outdir=${var_outdir_root}/luabind_lua504
elif [ "${var_lua_ver}" = "jit501" ]; then
     var_outdir=${var_outdir_root}/luabind_luajit21
else
     var_outdir=${var_outdir_root}/luabind
fi

var_outdir_include=${var_outdir}/include
var_outdir_lib=${var_outdir}/lib

rm -fr ${var_outdir_root}

mkdir -p "${var_outdir_include}"
mkdir -p "${var_outdir_lib}"

#${var_ndk_build_cmd} -j12 NDK_DEBUG=1 \
#    NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_luabind.mk APP_BUILD_SCRIPT=jni/Android_luabind_a.mk \
#    USRDEF_APP_LUA_VERSION=${var_lua_ver} USRDEF_APP_CPP_VER=${var_cpp_ver} \
#    V=1 > yggr_jni.log 2>&1

${var_ndk_build_cmd} -j12 NDK_DEBUG=1 \
    NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_luabind.mk APP_BUILD_SCRIPT=jni/Android_luabind_a.mk \
    USRDEF_APP_LUA_VERSION=${var_lua_ver} USRDEF_APP_CPP_VER=${var_cpp_ver}
${var_ndk_build_cmd} -j12 NDK_DEBUG=0 \
    NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_luabind.mk APP_BUILD_SCRIPT=jni/Android_luabind_a.mk \
    USRDEF_APP_LUA_VERSION=${var_lua_ver} USRDEF_APP_CPP_VER=${var_cpp_ver}

find ${var_middir_local} -type d -name 'objs*' -exec rm -fr {} \;

cp -fr ${var_middir_local}/* ${var_outdir_lib}/

${var_ndk_build_cmd} -j12 NDK_DEBUG=1 \
    NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_luabind.mk APP_BUILD_SCRIPT=jni/Android_luabind_so.mk \
    USRDEF_APP_LUA_VERSION=${var_lua_ver} USRDEF_APP_CPP_VER=${var_cpp_ver}
${var_ndk_build_cmd} -j12 NDK_DEBUG=0 \
    NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_luabind.mk APP_BUILD_SCRIPT=jni/Android_luabind_so.mk \
    USRDEF_APP_LUA_VERSION=${var_lua_ver} USRDEF_APP_CPP_VER=${var_cpp_ver}

${var_ndk_build_cmd} -j12 NDK_DEBUG=1 \
    NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_luabind.mk APP_BUILD_SCRIPT=jni/Android_luabind_exe.mk \
    USRDEF_APP_LUA_VERSION=${var_lua_ver} USRDEF_APP_CPP_VER=${var_cpp_ver}
${var_ndk_build_cmd} -j12 NDK_DEBUG=0 \
    NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_luabind.mk APP_BUILD_SCRIPT=jni/Android_luabind_exe.mk \
    USRDEF_APP_LUA_VERSION=${var_lua_ver} USRDEF_APP_CPP_VER=${var_cpp_ver}

touch ${var_outdir_lib}/arm64-v8a/mark-arm64-v8a.txt
touch ${var_outdir_lib}/armeabi-v7a/mark-armeabi-v7a.txt

if [ ! "${var_lua_ver}" = "jit501" ]; then
     touch ${var_outdir_lib}/riscv64/mark-riscv64.txt
fi

touch ${var_outdir_lib}/x86_64/mark-x86_64.txt
touch ${var_outdir_lib}/x86/mark-x86.txt

cp -f jni_lib_format.bat ${var_outdir}/