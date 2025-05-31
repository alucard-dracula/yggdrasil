#nsql_database_system_c_mongo_read_prefs_test CMakeLists.txt

project(nsql_database_system_c_mongo_read_prefs_test)

message(STATUS "build-sub-project [ nsql_database_system_c_mongo_read_prefs_test ]")

set(var_proj_name nsql_database_system_c_mongo_read_prefs_test)

set(var_root_proj_dir ${PROJECT_SOURCE_DIR}/../../..)
set(var_proj_src_dir ${var_root_proj_dir}/test)

set(var_proj_src_list 
		${var_proj_src_dir}/nsql_database_system/c_mongo_read_prefs_test.cpp
)

set(var_proj_bin_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_BIN_OUTDIR})
set(var_proj_lib_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_LIB_OUTDIR})
set(var_proj_ach_outdir ${var_proj_lib_outdir})

set(var_proj_include_dirs 
		${var_proj_src_dir}
		${var_proj_src_dir}/../
		${var_proj_src_dir}/../yggr_lua_base_type
		${USRDEF_CMAKE_INCLUDE_DIRS}
)

set(var_proj_c_defineds "${USRDEF_CMAKE_DEFINEDS}")

set(var_proj_c_defineds_debug "-DYGGR_SYSTEM_64 -D_DEBUG ${USRDEF_CMAKE_DEFINEDS_DEBUG}")

set(var_proj_c_defineds_release "-DNDEBUG -DYGGR_SYSTEM_64 ${USRDEF_CMAKE_DEFINEDS_RELEASE}")

set(var_proj_c_flags "-Wall -Werror=endif-labels -Werror=reorder -Werror=return-type -Wno-comment -Wno-enum-compare -Wno-multichar -Wno-sign-compare -Wno-unused-local-typedefs -fdata-sections -fexceptions -ffunction-sections -fno-strict-aliasing -ftemplate-backtrace-limit=0 ${USRDEF_CMAKE_CFLAGS}")

set(var_proj_c_flags_debug "-O0 -g ${USRDEF_CMAKE_CFLAGS_DEBUG}")

set(var_proj_c_flags_release "-O3 ${USRDEF_CMAKE_CFLAGS_RELEASE}")

set(var_proj_cxx_flags 
		-frtti
		${USRDEF_CMAKE_CXXFLAGS}
)

set(var_proj_cxx_flags_debug 
		-fno-elide-constructors
		${USRDEF_CMAKE_CXXFLAGS_DEBUG}
)

set(var_proj_cxx_flags_release 
		${USRDEF_CMAKE_CXXFLAGS_RELEASE}
)

set(var_proj_link_libs_dirs 
		${var_proj_ach_outdir}
		${USRDEF_CMAKE_LD_LIBRARY_PATH}
)

set(var_proj_link_libs 
		-lz
		-lssl
		-lsasl2
		-liconv
		-lcrypto
		-lresolv
		-lm
		${USRDEF_CMAKE_LD_LIBRARY}
)

if(${USRDEF_HAS_X86_64})
	set(var_proj_link_libs_debug 
			-lyggr_nsql_database_system${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lyggr_charset${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lyggr_base${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lyggr_compatibility${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lboost_serialization${USRDEF_CMAKE_COMPILER_VERSION_TAG}-mt-sd${USRDEF_CMAKE_BUILD_ARCH_BITS_TAG}${USRDEF_CMAKE_BOOST_VERSION_TAG}
			-lboost_thread${USRDEF_CMAKE_COMPILER_VERSION_TAG}-mt${USRDEF_CMAKE_BOOST_LINK_TAG_DEBUG}${USRDEF_CMAKE_BUILD_ARCH_BITS_TAG}${USRDEF_CMAKE_BOOST_VERSION_TAG}
			-lmongoc${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lmongocrypt${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lkms-message${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lbson${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lcommon${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lbson${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lcommon${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lutf8proc${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-licuuc
			-licui18n
			-licudata
			-lintel_dfp_obj${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lbase64${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			${USRDEF_CMAKE_LDFLAGS_DEBUG}
	)
else()
	set(var_proj_link_libs_debug 
			-lyggr_nsql_database_system${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lyggr_charset${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lyggr_base${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lyggr_compatibility${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lboost_serialization${USRDEF_CMAKE_COMPILER_VERSION_TAG}-mt-sd${USRDEF_CMAKE_BUILD_ARCH_BITS_TAG}${USRDEF_CMAKE_BOOST_VERSION_TAG}
			-lboost_thread${USRDEF_CMAKE_COMPILER_VERSION_TAG}-mt${USRDEF_CMAKE_BOOST_LINK_TAG_DEBUG}${USRDEF_CMAKE_BUILD_ARCH_BITS_TAG}${USRDEF_CMAKE_BOOST_VERSION_TAG}
			-lmongoc${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lmongocrypt${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lkms-message${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lbson${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lcommon${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lbson${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lcommon${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-lutf8proc${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			-licuuc
			-licui18n
			-licudata
			-lbase64${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
			${USRDEF_CMAKE_LDFLAGS_DEBUG}
	)
endif()

if(${USRDEF_HAS_X86_64})
	set(var_proj_link_libs_release 
			-lyggr_nsql_database_system${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lyggr_charset${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lyggr_base${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lyggr_compatibility${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lboost_serialization${USRDEF_CMAKE_COMPILER_VERSION_TAG}-mt-s${USRDEF_CMAKE_BUILD_ARCH_BITS_TAG}${USRDEF_CMAKE_BOOST_VERSION_TAG}
			-lboost_thread${USRDEF_CMAKE_COMPILER_VERSION_TAG}-mt${USRDEF_CMAKE_BOOST_LINK_TAG_RELEASE}${USRDEF_CMAKE_BUILD_ARCH_BITS_TAG}${USRDEF_CMAKE_BOOST_VERSION_TAG}
			-lmongoc${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lmongocrypt${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lkms-message${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lbson${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lcommon${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lbson${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lcommon${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lutf8proc${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-licuuc
			-licui18n
			-licudata
			-lintel_dfp_obj${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lbase64${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			${USRDEF_CMAKE_LD_LIBRARY_RELEASE}
	)
else()
	set(var_proj_link_libs_release 
			-lyggr_nsql_database_system${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lyggr_charset${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lyggr_base${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lyggr_compatibility${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lboost_serialization${USRDEF_CMAKE_COMPILER_VERSION_TAG}-mt-s${USRDEF_CMAKE_BUILD_ARCH_BITS_TAG}${USRDEF_CMAKE_BOOST_VERSION_TAG}
			-lboost_thread${USRDEF_CMAKE_COMPILER_VERSION_TAG}-mt${USRDEF_CMAKE_BOOST_LINK_TAG_RELEASE}${USRDEF_CMAKE_BUILD_ARCH_BITS_TAG}${USRDEF_CMAKE_BOOST_VERSION_TAG}
			-lmongoc${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lmongocrypt${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lkms-message${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lbson${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lcommon${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lbson${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lcommon${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-lutf8proc${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			-licuuc
			-licui18n
			-licudata
			-lbase64${USRDEF_CMAKE_COMPILER_VERSION_TAG}
			${USRDEF_CMAKE_LD_LIBRARY_RELEASE}
	)
endif()

set(var_proj_ldflags 
		${USRDEF_CMAKE_LDFLAGS}
)

set(var_proj_ldflags_debug 
		${USRDEF_CMAKE_LDFLAGS_DEBUG}
)

set(var_proj_ldflags_release 
		-Wl,--gc-sections
		${USRDEF_CMAKE_LDFLAGS_RELEASE}
)

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	set(var_proj_out_name ${var_proj_name})
	set(var_proj_c_defineds "${var_proj_c_defineds_debug} ${var_proj_c_defineds}")
	set(var_proj_c_flags "${var_proj_c_flags_debug} ${var_proj_c_flags}")
	set(var_proj_cxx_flags "${var_proj_cxx_flags_debug} ${var_proj_cxx_flags} ${var_proj_c_defineds}")
	set(var_proj_link_libs ${var_proj_link_libs_debug} ${var_proj_link_libs})
	set(var_proj_ldflags "${var_proj_ldflags_debug} ${var_proj_ldflags}")
else()
	set(var_proj_out_name ${var_proj_name})
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

add_executable(${var_proj_name} ${var_proj_src_list})

target_link_libraries(${var_proj_name} ${var_proj_link_libs})
set_target_properties( 
	${var_proj_name}
	PROPERTIES
	OUTPUT_NAME ${var_proj_out_name}
	RUNTIME_OUTPUT_DIRECTORY ${var_proj_bin_outdir}
	LIBRARY_OUTPUT_DIRECTORY ${var_proj_lib_outdir}
	ARCHIVE_OUTPUT_DIRECTORY ${var_proj_ach_outdir}
)

