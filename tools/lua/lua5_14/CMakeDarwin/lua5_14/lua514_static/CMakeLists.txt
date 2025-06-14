#lua514_static CMakeLists.txt

project(lua514_static)

message(STATUS "build-sub-project [ lua514_static ]")

set(var_proj_name lua514_static)

set(var_root_proj_dir ${PROJECT_SOURCE_DIR}/../../..)
set(var_proj_src_dir ${var_root_proj_dir}/lua514_static)

set(var_proj_src_list 
		${var_proj_src_dir}/../lua514/lapi.c
		${var_proj_src_dir}/../lua514/lauxlib.c
		${var_proj_src_dir}/../lua514/lbaselib.c
		${var_proj_src_dir}/../lua514/lcode.c
		${var_proj_src_dir}/../lua514/ldblib.c
		${var_proj_src_dir}/../lua514/ldebug.c
		${var_proj_src_dir}/../lua514/ldo.c
		${var_proj_src_dir}/../lua514/ldump.c
		${var_proj_src_dir}/../lua514/lfunc.c
		${var_proj_src_dir}/../lua514/lgc.c
		${var_proj_src_dir}/../lua514/linit.c
		${var_proj_src_dir}/../lua514/liolib.c
		${var_proj_src_dir}/../lua514/llex.c
		${var_proj_src_dir}/../lua514/lmathlib.c
		${var_proj_src_dir}/../lua514/lmem.c
		${var_proj_src_dir}/../lua514/loadlib.c
		${var_proj_src_dir}/../lua514/lobject.c
		${var_proj_src_dir}/../lua514/lopcodes.c
		${var_proj_src_dir}/../lua514/loslib.c
		${var_proj_src_dir}/../lua514/lparser.c
		${var_proj_src_dir}/../lua514/lstate.c
		${var_proj_src_dir}/../lua514/lstring.c
		${var_proj_src_dir}/../lua514/lstrlib.c
		${var_proj_src_dir}/../lua514/ltable.c
		${var_proj_src_dir}/../lua514/ltablib.c
		${var_proj_src_dir}/../lua514/ltm.c
		${var_proj_src_dir}/../lua514/lundump.c
		${var_proj_src_dir}/../lua514/lvm.c
		${var_proj_src_dir}/../lua514/lzio.c
		${var_proj_src_dir}/../lua514/print.c
)

set(var_proj_bin_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_BIN_OUTDIR})
set(var_proj_lib_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_LIB_OUTDIR})
set(var_proj_ach_outdir ${var_proj_lib_outdir})

set(var_proj_include_dirs 
		${var_proj_src_dir}
		${var_proj_src_dir}/../include
		${USRDEF_CMAKE_INCLUDE_DIRS}
)

set(var_proj_c_defineds "-DLUA_USELONGLONG -DLUA_USE_MACOSX ${USRDEF_CMAKE_DEFINEDS}")

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
	set(var_proj_out_name lua501${USRDEF_CMAKE_COMPILER_VERSION_TAG}-s-d)
	set(var_proj_c_defineds "${var_proj_c_defineds_debug} ${var_proj_c_defineds}")
	set(var_proj_c_flags "${var_proj_c_flags_debug} ${var_proj_c_flags}")
	set(var_proj_cxx_flags "${var_proj_cxx_flags_debug} ${var_proj_cxx_flags} ${var_proj_c_defineds}")
	set(var_proj_link_libs ${var_proj_link_libs_debug} ${var_proj_link_libs})
	set(var_proj_ldflags "${var_proj_ldflags_debug} ${var_proj_ldflags}")
else()
	set(var_proj_out_name lua501${USRDEF_CMAKE_COMPILER_VERSION_TAG}-s)
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

add_library(${var_proj_name} ${var_proj_src_list})

set_target_properties( 
	${var_proj_name}
	PROPERTIES
	OUTPUT_NAME ${var_proj_out_name}
	RUNTIME_OUTPUT_DIRECTORY ${var_proj_bin_outdir}
	LIBRARY_OUTPUT_DIRECTORY ${var_proj_lib_outdir}
	ARCHIVE_OUTPUT_DIRECTORY ${var_proj_ach_outdir}
)

