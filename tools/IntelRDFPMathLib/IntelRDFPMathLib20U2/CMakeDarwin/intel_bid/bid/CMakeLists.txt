#bid CMakeLists.txt

project(bid)

message(STATUS "build-sub-project [ bid ]")

set(var_proj_name bid)

set(var_root_proj_dir ${PROJECT_SOURCE_DIR}/../../..)
set(var_proj_src_dir ${var_root_proj_dir}/.)

set(var_proj_src_list 
		${var_proj_src_dir}/./LIBRARY/float128/dpml_exception.c
		${var_proj_src_dir}/./LIBRARY/float128/dpml_four_over_pi.c
		${var_proj_src_dir}/./LIBRARY/float128/dpml_ux_bessel.c
		${var_proj_src_dir}/./LIBRARY/float128/dpml_ux_bid.c
		${var_proj_src_dir}/./LIBRARY/float128/dpml_ux_cbrt.c
		${var_proj_src_dir}/./LIBRARY/float128/dpml_ux_erf.c
		${var_proj_src_dir}/./LIBRARY/float128/dpml_ux_exp.c
		${var_proj_src_dir}/./LIBRARY/float128/dpml_ux_int.c
		${var_proj_src_dir}/./LIBRARY/float128/dpml_ux_inv_hyper.c
		${var_proj_src_dir}/./LIBRARY/float128/dpml_ux_inv_trig.c
		${var_proj_src_dir}/./LIBRARY/float128/dpml_ux_lgamma.c
		${var_proj_src_dir}/./LIBRARY/float128/dpml_ux_log.c
		${var_proj_src_dir}/./LIBRARY/float128/dpml_ux_mod.c
		${var_proj_src_dir}/./LIBRARY/float128/dpml_ux_ops.c
		${var_proj_src_dir}/./LIBRARY/float128/dpml_ux_ops_64.c
		${var_proj_src_dir}/./LIBRARY/float128/dpml_ux_pow.c
		${var_proj_src_dir}/./LIBRARY/float128/dpml_ux_powi.c
		${var_proj_src_dir}/./LIBRARY/float128/dpml_ux_sqrt.c
		${var_proj_src_dir}/./LIBRARY/float128/dpml_ux_trig.c
		${var_proj_src_dir}/./LIBRARY/float128/sqrt_tab_t.c
		${var_proj_src_dir}/./LIBRARY/src/bid128.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_2_str_tables.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_acos.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_acosh.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_add.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_asin.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_asinh.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_atan.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_atan2.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_atanh.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_cbrt.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_compare.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_cos.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_cosh.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_div.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_erf.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_erfc.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_exp.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_exp10.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_exp2.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_expm1.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_fdimd.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_fma.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_fmod.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_frexp.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_hypot.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_ldexp.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_lgamma.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_llrintd.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_log.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_log10.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_log1p.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_log2.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_logb.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_logbd.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_lrintd.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_lround.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_minmax.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_modf.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_mul.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_nearbyintd.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_next.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_nexttowardd.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_noncomp.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_pow.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_quantexpd.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_quantize.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_rem.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_round_integral.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_scalb.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_scalbl.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_sin.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_sinh.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_sqrt.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_string.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_tan.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_tanh.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_tgamma.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_to_int16.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_to_int32.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_to_int64.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_to_int8.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_to_uint16.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_to_uint32.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_to_uint64.c
		${var_proj_src_dir}/./LIBRARY/src/bid128_to_uint8.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_acos.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_acosh.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_add.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_asin.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_asinh.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_atan.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_atan2.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_atanh.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_cbrt.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_compare.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_cos.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_cosh.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_div.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_erf.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_erfc.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_exp.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_exp10.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_exp2.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_expm1.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_fdimd.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_fma.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_fmod.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_frexp.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_hypot.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_ldexp.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_lgamma.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_llrintd.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_log.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_log10.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_log1p.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_log2.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_logb.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_logbd.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_lrintd.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_lround.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_minmax.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_modf.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_mul.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_nearbyintd.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_next.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_nexttowardd.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_noncomp.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_pow.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_quantexpd.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_quantize.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_rem.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_round_integral.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_scalb.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_scalbl.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_sin.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_sinh.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_sqrt.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_string.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_sub.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_tan.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_tanh.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_tgamma.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_to_bid128.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_to_bid64.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_to_int16.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_to_int32.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_to_int64.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_to_int8.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_to_uint16.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_to_uint32.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_to_uint64.c
		${var_proj_src_dir}/./LIBRARY/src/bid32_to_uint8.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_acos.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_acosh.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_add.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_asin.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_asinh.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_atan.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_atan2.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_atanh.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_cbrt.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_compare.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_cos.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_cosh.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_div.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_erf.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_erfc.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_exp.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_exp10.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_exp2.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_expm1.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_fdimd.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_fma.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_fmod.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_frexp.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_hypot.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_ldexp.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_lgamma.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_llrintd.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_log.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_log10.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_log1p.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_log2.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_logb.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_logbd.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_lrintd.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_lround.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_minmax.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_modf.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_mul.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_nearbyintd.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_next.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_nexttowardd.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_noncomp.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_pow.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_quantexpd.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_quantize.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_rem.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_round_integral.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_scalb.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_scalbl.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_sin.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_sinh.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_sqrt.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_string.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_tan.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_tanh.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_tgamma.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_to_bid128.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_to_int16.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_to_int32.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_to_int64.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_to_int8.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_to_uint16.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_to_uint32.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_to_uint64.c
		${var_proj_src_dir}/./LIBRARY/src/bid64_to_uint8.c
		${var_proj_src_dir}/./LIBRARY/src/bid_binarydecimal.c
		${var_proj_src_dir}/./LIBRARY/src/bid_convert_data.c
		${var_proj_src_dir}/./LIBRARY/src/bid_decimal_data.c
		${var_proj_src_dir}/./LIBRARY/src/bid_decimal_globals.c
		${var_proj_src_dir}/./LIBRARY/src/bid_dpd.c
		${var_proj_src_dir}/./LIBRARY/src/bid_feclearexcept.c
		${var_proj_src_dir}/./LIBRARY/src/bid_fegetexceptflag.c
		${var_proj_src_dir}/./LIBRARY/src/bid_feraiseexcept.c
		${var_proj_src_dir}/./LIBRARY/src/bid_fesetexceptflag.c
		${var_proj_src_dir}/./LIBRARY/src/bid_fetestexcept.c
		${var_proj_src_dir}/./LIBRARY/src/bid_flag_operations.c
		${var_proj_src_dir}/./LIBRARY/src/bid_from_int.c
		${var_proj_src_dir}/./LIBRARY/src/bid_round.c
		${var_proj_src_dir}/./LIBRARY/src/strtod128.c
		${var_proj_src_dir}/./LIBRARY/src/strtod32.c
		${var_proj_src_dir}/./LIBRARY/src/strtod64.c
		${var_proj_src_dir}/./LIBRARY/src/wcstod128.c
		${var_proj_src_dir}/./LIBRARY/src/wcstod32.c
		${var_proj_src_dir}/./LIBRARY/src/wcstod64.c
		${var_proj_src_dir}/LIBRARY/src/bid128_llquantexpd.c
		${var_proj_src_dir}/LIBRARY/src/bid128_llround.c
		${var_proj_src_dir}/LIBRARY/src/bid128_quantumd.c
		${var_proj_src_dir}/LIBRARY/src/bid32_llquantexpd.c
		${var_proj_src_dir}/LIBRARY/src/bid32_llround.c
		${var_proj_src_dir}/LIBRARY/src/bid32_quantumd.c
		${var_proj_src_dir}/LIBRARY/src/bid64_llquantexpd.c
		${var_proj_src_dir}/LIBRARY/src/bid64_llround.c
		${var_proj_src_dir}/LIBRARY/src/bid64_quantumd.c
)

set(var_proj_bin_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_BIN_OUTDIR})
set(var_proj_lib_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_LIB_OUTDIR})
set(var_proj_ach_outdir ${var_proj_lib_outdir})

set(var_proj_include_dirs 
		${var_proj_src_dir}
		${var_proj_src_dir}/./LIBRARY/float128
		${var_proj_src_dir}/./LIBRARY/src
		${USRDEF_CMAKE_INCLUDE_DIRS}
)

set(var_proj_c_defineds "${USRDEF_CMAKE_DEFINEDS}")

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

