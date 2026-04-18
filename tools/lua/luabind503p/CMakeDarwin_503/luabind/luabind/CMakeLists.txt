#luabind CMakeLists.txt

project(luabind)

message(STATUS "build-sub-project [ luabind ]")

set(var_proj_name luabind)

set(var_root_proj_dir ${PROJECT_SOURCE_DIR}/../../..)
set(var_proj_src_dir ${var_root_proj_dir}/luabind)

set(var_proj_src_list 
		${var_proj_src_dir}/class.cpp
		${var_proj_src_dir}/class_info.cpp
		${var_proj_src_dir}/class_registry.cpp
		${var_proj_src_dir}/class_rep.cpp
		${var_proj_src_dir}/create_class.cpp
		${var_proj_src_dir}/dllmain.cpp
		${var_proj_src_dir}/error.cpp
		${var_proj_src_dir}/exception_handler.cpp
		${var_proj_src_dir}/function.cpp
		${var_proj_src_dir}/inheritance.cpp
		${var_proj_src_dir}/link_compatibility.cpp
		${var_proj_src_dir}/object_rep.cpp
		${var_proj_src_dir}/open.cpp
		${var_proj_src_dir}/pcall.cpp
		${var_proj_src_dir}/scope.cpp
		${var_proj_src_dir}/stack_content_by_name.cpp
		${var_proj_src_dir}/weak_ref.cpp
		${var_proj_src_dir}/wrapper_base.cpp
)

set(var_proj_bin_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_BIN_OUTDIR})
set(var_proj_lib_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_LIB_OUTDIR})
set(var_proj_ach_outdir ${var_proj_lib_outdir})

set(var_proj_include_dirs 
		${var_proj_src_dir}
		${var_proj_src_dir}/../include
		${USRDEF_CMAKE_INCLUDE_DIRS}
)

set(var_proj_c_defineds "-DBUILD_DLL -DLUABIND_DYNAMIC_LINK -DLUABIND_EXPORT ${USRDEF_CMAKE_DEFINEDS}")

set(var_proj_c_defineds_debug "-D_DEBUG ${USRDEF_CMAKE_DEFINEDS_DEBUG}")

set(var_proj_c_defineds_release "-DNDEBUG ${USRDEF_CMAKE_DEFINEDS_RELEASE}")

set(var_proj_c_flags "-Wall -Werror=endif-labels -Werror=reorder -Werror=return-type -Wno-comment -Wno-enum-compare -Wno-multichar -Wno-sign-compare -Wno-unused-local-typedefs -fPIC -fdata-sections -fexceptions -ffunction-sections -fno-strict-aliasing -ftemplate-backtrace-limit=0 ${USRDEF_CMAKE_CFLAGS}")

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
		${USRDEF_CMAKE_LD_LIBRARY}
)

set(var_proj_link_libs_debug 
		-llua503${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d
		${USRDEF_CMAKE_LD_LIBRARY_DEBUG}
)

set(var_proj_link_libs_release 
		-llua503${USRDEF_CMAKE_COMPILER_VERSION_TAG}
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
	set(var_proj_out_name luabind09${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d)
	set(var_proj_c_defineds "${var_proj_c_defineds_debug} ${var_proj_c_defineds}")
	set(var_proj_c_flags "${var_proj_c_flags_debug} ${var_proj_c_flags}")
	set(var_proj_cxx_flags "${var_proj_cxx_flags_debug} ${var_proj_cxx_flags} ${var_proj_c_defineds}")
	set(var_proj_link_libs ${var_proj_link_libs_debug} ${var_proj_link_libs})
	set(var_proj_ldflags "${var_proj_ldflags_debug} ${var_proj_ldflags}")
else()
	set(var_proj_out_name luabind09${USRDEF_CMAKE_COMPILER_VERSION_TAG})
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

