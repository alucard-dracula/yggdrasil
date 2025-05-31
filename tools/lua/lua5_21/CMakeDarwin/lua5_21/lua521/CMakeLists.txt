#lua521 CMakeLists.txt

project(lua521)

message(STATUS "build-sub-project [ lua521 ]")

set(var_proj_name lua521)

set(var_root_proj_dir ${PROJECT_SOURCE_DIR}/../../..)
set(var_proj_src_dir ${var_root_proj_dir}/lua521)

set(var_proj_src_list 
		${var_proj_src_dir}/lapi.c
		${var_proj_src_dir}/lauxlib.c
		${var_proj_src_dir}/lbaselib.c
		${var_proj_src_dir}/lbitlib.c
		${var_proj_src_dir}/lcode.c
		${var_proj_src_dir}/lcorolib.c
		${var_proj_src_dir}/lctype.c
		${var_proj_src_dir}/ldblib.c
		${var_proj_src_dir}/ldebug.c
		${var_proj_src_dir}/ldo.c
		${var_proj_src_dir}/ldump.c
		${var_proj_src_dir}/lfunc.c
		${var_proj_src_dir}/lgc.c
		${var_proj_src_dir}/linit.c
		${var_proj_src_dir}/liolib.c
		${var_proj_src_dir}/llex.c
		${var_proj_src_dir}/lmathlib.c
		${var_proj_src_dir}/lmem.c
		${var_proj_src_dir}/loadlib.c
		${var_proj_src_dir}/lobject.c
		${var_proj_src_dir}/lopcodes.c
		${var_proj_src_dir}/loslib.c
		${var_proj_src_dir}/lparser.c
		${var_proj_src_dir}/lstate.c
		${var_proj_src_dir}/lstring.c
		${var_proj_src_dir}/lstrlib.c
		${var_proj_src_dir}/ltable.c
		${var_proj_src_dir}/ltablib.c
		${var_proj_src_dir}/ltm.c
		${var_proj_src_dir}/lundump.c
		${var_proj_src_dir}/lvm.c
		${var_proj_src_dir}/lzio.c
)

set(var_proj_bin_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_BIN_OUTDIR})
set(var_proj_lib_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_LIB_OUTDIR})
set(var_proj_ach_outdir ${var_proj_lib_outdir})

set(var_proj_include_dirs 
		${var_proj_src_dir}
		${var_proj_src_dir}/../include
		${USRDEF_CMAKE_INCLUDE_DIRS}
)

set(var_proj_c_defineds "-DBUILD_DLL -DLUA521_EXPORT -DLUA_COMPAT_ALL -DLUA_USE_MACOSX ${USRDEF_CMAKE_DEFINEDS}")

set(var_proj_c_defineds_debug "-D_DEBUG ${USRDEF_CMAKE_DEFINEDS_DEBUG}")

set(var_proj_c_defineds_release "-DNDEBUG ${USRDEF_CMAKE_DEFINEDS_RELEASE}")

set(var_proj_c_flags "-Wall -Werror=endif-labels -Werror=return-type -Wno-comment -Wno-enum-compare -Wno-multichar -Wno-sign-compare -Wno-unused-local-typedefs -fPIC -fdata-sections -fexceptions -ffunction-sections -fno-strict-aliasing ${USRDEF_CMAKE_CFLAGS}")

set(var_proj_c_flags_debug "-O0 -g ${USRDEF_CMAKE_CFLAGS_DEBUG}")

set(var_proj_c_flags_release "-O3 ${USRDEF_CMAKE_CFLAGS_RELEASE}")

set(var_proj_cxx_flags 
		-frtti
		${USRDEF_CMAKE_CXXFLAGS}
)

set(var_proj_cxx_flags_debug 
		${USRDEF_CMAKE_CXXFLAGS_DEBUG}
)

set(var_proj_cxx_flags_release 
		${USRDEF_CMAKE_CXXFLAGS_RELEASE}
)

set(var_proj_link_libs_dirs 
		${USRDEF_CMAKE_LD_LIBRARY_PATH}
)

set(var_proj_link_libs 
		-lm
		${USRDEF_CMAKE_LD_LIBRARY}
)

set(var_proj_link_libs_debug 
		${USRDEF_CMAKE_LDFLAGS_DEBUG}
)

set(var_proj_link_libs_release 
		${USRDEF_CMAKE_LD_LIBRARY_RELEASE}
)

set(var_proj_ldflags 
		${USRDEF_CMAKE_LDFLAGS}
)

set(var_proj_ldflags_debug 
		${USRDEF_CMAKE_LDFLAGS_DEBUG}
)

set(var_proj_ldflags_release 
		${USRDEF_CMAKE_LDFLAGS_RELEASE}
)

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	set(var_proj_out_name lua502${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d)
	set(var_proj_c_defineds "${var_proj_c_defineds_debug} ${var_proj_c_defineds}")
	set(var_proj_c_flags "${var_proj_c_flags_debug} ${var_proj_c_flags}")
	set(var_proj_cxx_flags "${var_proj_cxx_flags_debug} ${var_proj_cxx_flags} ${var_proj_c_defineds}")
	set(var_proj_link_libs ${var_proj_link_libs_debug} ${var_proj_link_libs})
	set(var_proj_ldflags "${var_proj_ldflags_debug} ${var_proj_ldflags}")
else()
	set(var_proj_out_name lua502${USRDEF_CMAKE_COMPILER_VERSION_TAG})
	set(var_proj_c_defineds "${var_proj_c_defineds_release} ${var_proj_c_defineds}")
	set(var_proj_c_flags "${var_proj_c_flags_release} ${var_proj_c_flags}")
	set(var_proj_cxx_flags "${var_proj_cxx_flags_release} ${var_proj_cxx_flags} ${var_proj_c_defineds}")
	set(var_proj_link_libs ${var_proj_link_libs_release} ${var_proj_link_libs})
	set(var_proj_ldflags "${var_proj_ldflags_release} ${var_proj_ldflags}")
endif()

include_directories(${var_proj_include_dirs})
add_definitions(${var_proj_c_defineds})
set(CMAKE_C_FLAGS ${var_proj_c_flags})
set(CMAKE_CXX_FLAGS ${var_proj_cxx_flags})

link_directories(${var_proj_link_libs_dirs})

add_library(${var_proj_name} SHARED ${var_proj_src_list})

set_target_properties( 
	${var_proj_name}
	PROPERTIES
	SUFFIX ".dylib"
)

set_target_properties( 
	${var_proj_name}
	PROPERTIES
	IMPORTED_IMPLIB "lib${var_proj_out_name}.a"
)

target_link_libraries(${var_proj_name} ${var_proj_link_libs})
set_target_properties( 
	${var_proj_name}
	PROPERTIES
	OUTPUT_NAME ${var_proj_out_name}
	RUNTIME_OUTPUT_DIRECTORY ${var_proj_bin_outdir}
	LIBRARY_OUTPUT_DIRECTORY ${var_proj_lib_outdir}
	ARCHIVE_OUTPUT_DIRECTORY ${var_proj_ach_outdir}
)

