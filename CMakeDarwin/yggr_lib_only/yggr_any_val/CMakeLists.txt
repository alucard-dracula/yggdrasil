#yggr_any_val CMakeLists.txt

project(yggr_any_val)

message(STATUS "build-sub-project [ yggr_any_val ]")

set(var_proj_name yggr_any_val)

set(var_root_proj_dir ${PROJECT_SOURCE_DIR}/../../..)
set(var_proj_src_dir ${var_root_proj_dir}/yggr_any_val)

set(var_proj_src_list 
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_bitwise_op_and_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_bitwise_op_and_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_bitwise_op_left_shift_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_bitwise_op_left_shift_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_bitwise_op_not.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_bitwise_op_or_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_bitwise_op_or_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_bitwise_op_right_shift_enis.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_bitwise_op_right_shift_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_bitwise_op_xor_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_bitwise_op_xor_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_logic_op_and.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_logic_op_equal_to_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_logic_op_equal_to_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_logic_op_greater_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_logic_op_greater_equal_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_logic_op_greater_equal_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_logic_op_greater_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_logic_op_less_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_logic_op_less_equal_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_logic_op_less_equal_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_logic_op_less_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_logic_op_not.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_logic_op_not_equal_to_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_logic_op_not_equal_to_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_logic_op_or.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_math_bin_op_div_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_math_bin_op_div_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_math_bin_op_minus_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_math_bin_op_minus_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_math_bin_op_mod_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_math_bin_op_mod_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_math_bin_op_mul_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_math_bin_op_mul_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_math_bin_op_plus_drei.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_math_bin_op_plus_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_math_bin_op_plus_funf.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_math_bin_op_plus_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_math_bin_op_set_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_math_bin_op_set_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_math_unary_op_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_basic_t_reg_math_unary_op_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_bitwise_op_and_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_bitwise_op_and_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_bitwise_op_left_shift_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_bitwise_op_left_shift_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_bitwise_op_not.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_bitwise_op_or_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_bitwise_op_or_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_bitwise_op_right_shift_enis.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_bitwise_op_right_shift_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_bitwise_op_xor_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_bitwise_op_xor_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_logic_op_and.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_logic_op_equal_to_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_logic_op_equal_to_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_logic_op_greater_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_logic_op_greater_equal_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_logic_op_greater_equal_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_logic_op_greater_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_logic_op_less_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_logic_op_less_equal_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_logic_op_less_equal_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_logic_op_less_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_logic_op_not.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_logic_op_not_equal_to_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_logic_op_not_equal_to_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_logic_op_or.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_math_bin_op_div_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_math_bin_op_div_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_math_bin_op_minus_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_math_bin_op_minus_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_math_bin_op_mod_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_math_bin_op_mod_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_math_bin_op_mul_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_math_bin_op_mul_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_math_bin_op_plus_drei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_math_bin_op_plus_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_math_bin_op_plus_funf.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_math_bin_op_plus_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_math_bin_op_set_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_math_bin_op_set_zwei.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_math_unary_op_eins.cpp
		${var_proj_src_dir}/any_operator_mgr_boost_basic_t_reg_math_unary_op_zwei.cpp
)

set(var_proj_bin_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_BIN_OUTDIR})
set(var_proj_lib_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_LIB_OUTDIR})
set(var_proj_ach_outdir ${var_proj_lib_outdir})

set(var_proj_include_dirs 
		${var_proj_src_dir}
		${var_proj_src_dir}/../
		${USRDEF_CMAKE_INCLUDE_DIRS}
)

set(var_proj_c_defineds "${USRDEF_CMAKE_DEFINEDS}")

set(var_proj_c_defineds_debug "-DYGGR_SYSTEM_64 -D_DEBUG ${USRDEF_CMAKE_DEFINEDS_DEBUG}")

set(var_proj_c_defineds_release "-DNDEBUG -DYGGR_SYSTEM_64 ${USRDEF_CMAKE_DEFINEDS_RELEASE}")

set(var_proj_c_flags "-Wall -Werror=endif-labels -Werror=reorder -Werror=return-type -Wno-comment -Wno-enum-compare -Wno-multichar -Wno-sign-compare -Wno-unused-local-typedefs -fPIC -fdata-sections -fexceptions -ffunction-sections -fno-strict-aliasing -ftemplate-backtrace-limit=0 ${USRDEF_CMAKE_CFLAGS}")

set(var_proj_c_flags_debug "-Og ${USRDEF_CMAKE_CFLAGS_DEBUG}")

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
		${USRDEF_CMAKE_LD_LIBRARY_PATH}
)

set(var_proj_link_libs 
		${USRDEF_CMAKE_LD_LIBRARY}
)

set(var_proj_link_libs_debug 
		${USRDEF_CMAKE_LD_LIBRARY_DEBUG}
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
	set(var_proj_out_name ${var_proj_name}${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d)
	set(var_proj_c_defineds "${var_proj_c_defineds_debug} ${var_proj_c_defineds}")
	set(var_proj_c_flags "${var_proj_c_flags_debug} ${var_proj_c_flags}")
	set(var_proj_cxx_flags "${var_proj_cxx_flags_debug} ${var_proj_cxx_flags} ${var_proj_c_defineds}")
	set(var_proj_link_libs ${var_proj_link_libs_debug} ${var_proj_link_libs})
	set(var_proj_ldflags "${var_proj_ldflags_debug} ${var_proj_ldflags}")
else()
	set(var_proj_out_name ${var_proj_name}${USRDEF_CMAKE_COMPILER_VERSION_TAG})
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

