#yggr CMakeLists.txt

cmake_minimum_required(VERSION 3.12)

if(USRDEF_CMAKE_C_COMPILER)
	set(CMAKE_C_COMPILER ${USRDEF_CMAKE_C_COMPILER})
endif()

if(USRDEF_CMAKE_CXX_COMPILER)
	set(CMAKE_CXX_COMPILER ${USRDEF_CMAKE_CXX_COMPILER})
endif()

#set(USRDEF_CMAKE_COMPILER_VERSION "clang-darwin16")
#set(USRDEF_CMAKE_BUILD_LD_ARCH_TAG "a")
#set(USRDEF_CMAKE_BUILD_ARCH_BITS "64")
set(USRDEF_CMAKE_BOOST_VERSION "1_82")
set(USRDEF_CMAKE_PYTHON_VERSION "313")

set(USRDEF_CMAKE_PYTHON_INCLUDE_DIR "python3.13")

#set(USRDEF_CMAKE_OSX_SDK_NAME "macosx")

if("${USRDEF_CMAKE_OSX_SDK_NAME}" STREQUAL "macosx")
	set(USRDEF_CMAKE_BOOST_LINK_TAG_DEBUG "-d")
	set(USRDEF_CMAKE_BOOST_LINK_TAG_RELEASE "")
else()
	set(USRDEF_CMAKE_BOOST_LINK_TAG_DEBUG "-sd")
	set(USRDEF_CMAKE_BOOST_LINK_TAG_RELEASE "-s")
endif()

set(USRDEF_CMAKE_COMPILER_VERSION_TAG "-${USRDEF_CMAKE_COMPILER_VERSION}")
set(USRDEF_CMAKE_BUILD_ARCH_BITS_TAG "-${USRDEF_CMAKE_BUILD_LD_ARCH_TAG}${USRDEF_CMAKE_BUILD_ARCH_BITS}")

set(USRDEF_CMAKE_BOOST_VERSION_TAG "-${USRDEF_CMAKE_BOOST_VERSION}")

project(yggr)

message(STATUS "build-root-project [ yggr ]")

set(CMAKE_C_STANDARD ${USRDEF_CMAKE_C_STANDARD})
set(CMAKE_CXX_STANDARD ${USRDEF_CMAKE_CXX_STANDARD})
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE ${USRDEF_CMAKE_BUILD_TYPE})

set(CMAKE_OSX_SYSROOT ${USRDEF_CMAKE_OSX_SYSROOT})
set(CMAKE_OSX_ARCHITECTURES ${USRDEF_CMAKE_OSX_ARCHITECTURES})

string(FIND "${USRDEF_CMAKE_OSX_ARCHITECTURES}" "arm64" var_arm64_chk)
if(var_arm64_chk EQUAL -1)
	set(USRDEF_HAS_ARM64 OFF)
else()
	set(USRDEF_HAS_ARM64 ON)
endif()

if("${USRDEF_CMAKE_OSX_SDK_NAME}" STREQUAL "macosx")
	 set(var_third_part_dir "third_part")
else()
	 set(var_third_part_dir "third_part_${USRDEF_CMAKE_OSX_SDK_NAME}")
endif()

set(USRDEF_CMAKE_INCLUDE_DIRS 
		~/${var_third_part_dir}/boost/include
		~/${var_third_part_dir}/libbase64/include
		~/${var_third_part_dir}/libblowfish/include
		~/${var_third_part_dir}/libmd5/include
		~/${var_third_part_dir}/libsha/include
		~/${var_third_part_dir}/libicu/include
		~/${var_third_part_dir}/libzlib/include
		~/${var_third_part_dir}/libopenssl/include
		~/${var_third_part_dir}/LzmaLib/include
		~/${var_third_part_dir}/IntelRDFPMathLib/include
		~/${var_third_part_dir}/IntelRDFPMathLib/include/src
		~/${var_third_part_dir}/unixODBC/include
		~/${var_third_part_dir}/libdtl/include
		~/${var_third_part_dir}/mongodb/include
		~/${var_third_part_dir}/mongodb_c_driver/include
		~/${var_third_part_dir}/lua/include
		~/${var_third_part_dir}/luabind/include
		~/${var_third_part_dir}/python/include/${USRDEF_CMAKE_PYTHON_INCLUDE_DIR}
)

if(${USRDEF_HAS_ARM64})
	set(USRDEF_CMAKE_DEFINEDS "-DYGGR_TEST_DISABLE_NON_GENERIC_SUPPORT")
else()
	set(USRDEF_CMAKE_DEFINEDS "")
endif()
set(USRDEF_CMAKE_DEFINEDS_DEBUG "")
set(USRDEF_CMAKE_DEFINEDS_RELEASE "")

#set(USRDEF_CMAKE_CFLAGS "")
set(USRDEF_CMAKE_CFLAGS_DEBUG "")
set(USRDEF_CMAKE_CFLAGS_RELEASE "")

set(USRDEF_CMAKE_CXXFLAGS "")
set(USRDEF_CMAKE_CXXFLAGS_DEBUG "")
set(USRDEF_CMAKE_CXXFLAGS_RELEASE "")

set(USRDEF_CMAKE_LD_LIBRARY_PATH 
		~/${var_third_part_dir}/boost/lib
		~/${var_third_part_dir}/libbase64/lib
		~/${var_third_part_dir}/libblowfish/lib
		~/${var_third_part_dir}/libmd5/lib
		~/${var_third_part_dir}/libsha/lib
		~/${var_third_part_dir}/libicu/lib
		~/${var_third_part_dir}/libzlib/lib
		~/${var_third_part_dir}/libopenssl/lib
		~/${var_third_part_dir}/LzmaLib/lib
		~/${var_third_part_dir}/IntelRDFPMathLib/lib
		~/${var_third_part_dir}/unixODBC/lib
		~/${var_third_part_dir}/libdtl/lib
		~/${var_third_part_dir}/mongodb/lib
		~/${var_third_part_dir}/mongodb_c_driver/lib
		~/${var_third_part_dir}/lua/lib
		~/${var_third_part_dir}/luabind/lib
		~/${var_third_part_dir}/python/lib
)

set(USRDEF_CMAKE_LD_LIBRARY "")
set(USRDEF_CMAKE_LD_LIBRARY_DEBUG "")
set(USRDEF_CMAKE_LD_LIBRARY_RELEASE "")

set(USRDEF_CMAKE_LDFLAGS "")
set(USRDEF_CMAKE_LDFLAGS_DEBUG "")
set(USRDEF_CMAKE_LDFLAGS_RELEASE "")

set(USRDEF_CMAKE_PROJ_ROOT_DIR ${PROJECT_SOURCE_DIR})

if(${USRDEF_HAS_ARM64})
	set(var_proj_deps_a 
			yggr_any_val
			yggr_base
			yggr_charset
			yggr_compaction_tool
			yggr_compatibility
			yggr_database_system
			yggr_encryption_tool
			yggr_exception
			yggr_ids
			yggr_log
			yggr_modules
			yggr_script_lua
			yggr_segment
			yggr_seh
			yggr_system_controller
			yggr_time
	)
else()
	set(var_proj_deps_a 
			yggr_any_val
			yggr_base
			yggr_charset
			yggr_compaction_tool
			yggr_compatibility
			yggr_database_system
			yggr_encryption_tool
			yggr_exception
			yggr_ids
			yggr_log
			yggr_modules
			yggr_nsql_database_system
			yggr_script_lua
			yggr_segment
			yggr_seh
			yggr_system_controller
			yggr_time
	)
endif()

if("${USRDEF_CMAKE_OSX_SDK_NAME}" STREQUAL "macosx")
	list(APPEND var_proj_deps_a 
		yggr_script_python
	)
endif()

set(var_proj_deps_so 
		dll_cal_test
		lua_cal_svr_procotol
		lua_calculator
		lua_hello_dll
		yggr_lua_base_type
)

if("${USRDEF_CMAKE_OSX_SDK_NAME}" STREQUAL "macosx")
	list(APPEND var_proj_deps_so 
		pyd_export
	)
endif()

#add_subdirectory static libraries
add_subdirectory(yggr_any_val)
add_subdirectory(yggr_base)
add_subdirectory(yggr_charset)
add_subdirectory(yggr_compaction_tool)
add_subdirectory(yggr_compatibility)
add_subdirectory(yggr_database_system)
add_subdirectory(yggr_encryption_tool)
add_subdirectory(yggr_exception)
add_subdirectory(yggr_ids)
add_subdirectory(yggr_log)
add_subdirectory(yggr_modules)
add_subdirectory(yggr_script_lua)
add_subdirectory(yggr_segment)
add_subdirectory(yggr_seh)
add_subdirectory(yggr_system_controller)
add_subdirectory(yggr_time)

if(NOT ${USRDEF_HAS_ARM64})
	add_subdirectory(yggr_nsql_database_system)
endif()

if("${USRDEF_CMAKE_OSX_SDK_NAME}" STREQUAL "macosx")
	add_subdirectory(yggr_script_python)
endif()

#add_subdirectory dynamic libraries
add_subdirectory(dll_cal_test)
add_dependencies(dll_cal_test ${var_proj_deps_a})

add_subdirectory(lua_cal_svr_procotol)
add_dependencies(lua_cal_svr_procotol ${var_proj_deps_a})

add_subdirectory(lua_calculator)
add_dependencies(lua_calculator ${var_proj_deps_a})

add_subdirectory(lua_hello_dll)
add_dependencies(lua_hello_dll ${var_proj_deps_a})

add_subdirectory(yggr_lua_base_type)
add_dependencies(yggr_lua_base_type ${var_proj_deps_a})


if("${USRDEF_CMAKE_OSX_SDK_NAME}" STREQUAL "macosx")
	add_subdirectory(pyd_export)
	add_dependencies(pyd_export ${var_proj_deps_a})

endif()

#add_subdirectory executable
add_subdirectory(active_svr_test)
add_dependencies(active_svr_test ${var_proj_deps_so})

add_subdirectory(clt_test_async)
add_dependencies(clt_test_async ${var_proj_deps_so})

add_subdirectory(clt_test_static_task_center_balance)
add_dependencies(clt_test_static_task_center_balance ${var_proj_deps_so})

add_subdirectory(clt_test_static_task_center_balance2)
add_dependencies(clt_test_static_task_center_balance2 ${var_proj_deps_so})

add_subdirectory(clt_test_uuid_static_task_center_balance)
add_dependencies(clt_test_uuid_static_task_center_balance ${var_proj_deps_so})

add_subdirectory(encryption_tool_clt_test)
add_dependencies(encryption_tool_clt_test ${var_proj_deps_so})

add_subdirectory(encryption_tool_proxy_srv_test)
add_dependencies(encryption_tool_proxy_srv_test ${var_proj_deps_so})

add_subdirectory(encryption_tool_proxy_trun_srv_test)
add_dependencies(encryption_tool_proxy_trun_srv_test ${var_proj_deps_so})

add_subdirectory(encryption_tool_svr_test)
add_dependencies(encryption_tool_svr_test ${var_proj_deps_so})

add_subdirectory(lua_active_svr_test)
add_dependencies(lua_active_svr_test ${var_proj_deps_so})

add_subdirectory(lua_clt_test)
add_dependencies(lua_clt_test ${var_proj_deps_so})

add_subdirectory(mixer_svr_test)
add_dependencies(mixer_svr_test ${var_proj_deps_so})

add_subdirectory(p2p_boot_svr_test)
add_dependencies(p2p_boot_svr_test ${var_proj_deps_so})

add_subdirectory(p2p_peer_test)
add_dependencies(p2p_peer_test ${var_proj_deps_so})

add_subdirectory(proxy_srv_test)
add_dependencies(proxy_srv_test ${var_proj_deps_so})

add_subdirectory(proxy_trun_mixer_svr_test)
add_dependencies(proxy_trun_mixer_svr_test ${var_proj_deps_so})

add_subdirectory(proxy_trun_srv_test)
add_dependencies(proxy_trun_srv_test ${var_proj_deps_so})

add_subdirectory(seh_svr_test)
add_dependencies(seh_svr_test ${var_proj_deps_so})

add_subdirectory(svr_test_async)
add_dependencies(svr_test_async ${var_proj_deps_so})

add_subdirectory(svr_test_balance_async)
add_dependencies(svr_test_balance_async ${var_proj_deps_so})

add_subdirectory(svr_test_service_handler_shared_wrap_test)
add_dependencies(svr_test_service_handler_shared_wrap_test ${var_proj_deps_so})

add_subdirectory(svr_test_static_task_center_balance_async)
add_dependencies(svr_test_static_task_center_balance_async ${var_proj_deps_so})

add_subdirectory(svr_test_uuid_static_task_center_balance_async)
add_dependencies(svr_test_uuid_static_task_center_balance_async ${var_proj_deps_so})

add_subdirectory(tool_conv_to_cb_solution_linux)
add_dependencies(tool_conv_to_cb_solution_linux ${var_proj_deps_so})

add_subdirectory(tool_conv_to_cb_solution_win_mingw)
add_dependencies(tool_conv_to_cb_solution_win_mingw ${var_proj_deps_so})

add_subdirectory(tool_conv_to_darwin_cmakelists_txt)
add_dependencies(tool_conv_to_darwin_cmakelists_txt ${var_proj_deps_so})

add_subdirectory(tool_conv_to_jni_mk)
add_dependencies(tool_conv_to_jni_mk ${var_proj_deps_so})

add_subdirectory(tool_downgrade_vcxproj)
add_dependencies(tool_downgrade_vcxproj ${var_proj_deps_so})

add_subdirectory(tool_format_vcxproj)
add_dependencies(tool_format_vcxproj ${var_proj_deps_so})

add_subdirectory(tool_remove_duplicate_files)
add_dependencies(tool_remove_duplicate_files ${var_proj_deps_so})

add_subdirectory(tool_suo_batch_build_selector)
add_dependencies(tool_suo_batch_build_selector ${var_proj_deps_so})

add_subdirectory(tool_test_project_maker)
add_dependencies(tool_test_project_maker ${var_proj_deps_so})

add_subdirectory(tool_test_search_files)
add_dependencies(tool_test_search_files ${var_proj_deps_so})

add_subdirectory(tool_upgrade_vcxproj)
add_dependencies(tool_upgrade_vcxproj ${var_proj_deps_so})

add_subdirectory(udp_clt_test)
add_dependencies(udp_clt_test ${var_proj_deps_so})

add_subdirectory(udp_proxy_svr_test)
add_dependencies(udp_proxy_svr_test ${var_proj_deps_so})

add_subdirectory(udp_proxy_trun_svr_test)
add_dependencies(udp_proxy_trun_svr_test ${var_proj_deps_so})

add_subdirectory(udp_svr_test)
add_dependencies(udp_svr_test ${var_proj_deps_so})


