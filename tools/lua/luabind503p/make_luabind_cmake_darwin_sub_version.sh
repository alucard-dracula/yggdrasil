#!/bin/bash

make_luabind_cmake_darwin_sub_ver_detel(){
    #var_lua_ver(501, 502, 503, 504, jit501)
    var_lua_code_ver=$1 
    
    if [ "${var_lua_code_ver}" = "501" ]; then
        var_lua_real_ver=514
    elif [ "${var_lua_code_ver}" = "502" ]; then
        var_lua_real_ver=521
    elif [ "${var_lua_code_ver}" = "503" ]; then
        var_lua_real_ver=534
    elif [ "${var_lua_code_ver}" = "504" ]; then
        var_lua_real_ver=546
    elif [ "${var_lua_code_ver}" = "jit501" ]; then
        var_lua_real_ver=jit21
    else
        var_lua_real_ver=
    fi

    if [ "${var_lua_code_ver}" = "jit501" ]; then
        var_lua_title=${var_lua_real_ver}
    else
        var_lua_title=${var_lua_code_ver}
    fi

    var_src_name=CMakeDarwin
    var_dst_name=${var_src_name}_${var_lua_title}


    rm -fr "${var_dst_name}"
    cp -fr "${var_src_name}" "${var_dst_name}"

    find ${var_dst_name} -type f -name '*.txt' -exec sed -i "s/-llua504/-llua${var_lua_code_ver}/g" {} \;
    find ${var_dst_name} -type f -name '*.txt' -exec sed -i "s/\/lua\//\/lua_${var_lua_real_ver}\//g" {} \;
    find ${var_dst_name} -type f -name '*.sh' -exec sed -i "s/var_sln_name=luabind/var_sln_name=luabind_${var_lua_title}/g" {} \;

    if [ "${var_lua_code_ver}" = "jit501" ]; then
        iphoneos_mark_beg="#<< 'SH_IPHONEOS_BUILD_MARK'"
        iphoneos_mark_end="#SH_IPHONEOS_BUILD_MARK"

        find ${var_dst_name} -type f -name '*.sh' -exec sed -i "s/${iphoneos_mark_beg}/${iphoneos_mark_beg:1}/g" {} \;
        find ${var_dst_name} -type f -name '*.sh' -exec sed -i "s/${iphoneos_mark_end}/${iphoneos_mark_end:1}/g" {} \;

        iphonesimulator_mark_beg="#<< 'SH_IPHONESIMULATOR_BUILD_MARK'"
        iphonesimulator_mark_end="#SH_IPHONESIMULATOR_BUILD_MARK"
        find ${var_dst_name} -type f -name '*.sh' -exec sed -i "s/${iphonesimulator_mark_beg}/${iphonesimulator_mark_beg:1}/g" {} \;
        find ${var_dst_name} -type f -name '*.sh' -exec sed -i "s/${iphonesimulator_mark_end}/${iphonesimulator_mark_end:1}/g" {} \;
    fi

}

make_luabind_cmake_darwin_sub_ver_detel 501

make_luabind_cmake_darwin_sub_ver_detel 502

make_luabind_cmake_darwin_sub_ver_detel 503

make_luabind_cmake_darwin_sub_ver_detel 504

make_luabind_cmake_darwin_sub_ver_detel "jit501"

