#mongocrypt CMakeLists.txt

project(mongocrypt)

message(STATUS "build-sub-project [ mongocrypt ]")

set(var_proj_name mongocrypt)

set(var_root_proj_dir ${PROJECT_SOURCE_DIR}/../../..)
set(var_proj_src_dir ${var_root_proj_dir}/mongocrypt)

set(var_proj_src_list 
		${var_proj_src_dir}/crypto/cng.c
		${var_proj_src_dir}/crypto/commoncrypto.c
		${var_proj_src_dir}/crypto/libcrypto.c
		${var_proj_src_dir}/crypto/none.c
		${var_proj_src_dir}/mc-array.c
		${var_proj_src_dir}/mc-efc.c
		${var_proj_src_dir}/mc-fle2-encryption-placeholder.c
		${var_proj_src_dir}/mc-fle2-find-equality-payload-v2.c
		${var_proj_src_dir}/mc-fle2-find-equality-payload.c
		${var_proj_src_dir}/mc-fle2-find-range-payload-v2.c
		${var_proj_src_dir}/mc-fle2-find-range-payload.c
		${var_proj_src_dir}/mc-fle2-insert-update-payload-v2.c
		${var_proj_src_dir}/mc-fle2-insert-update-payload.c
		${var_proj_src_dir}/mc-fle2-payload-iev-v2.c
		${var_proj_src_dir}/mc-fle2-payload-iev.c
		${var_proj_src_dir}/mc-fle2-payload-uev-common.c
		${var_proj_src_dir}/mc-fle2-payload-uev-v2.c
		${var_proj_src_dir}/mc-fle2-payload-uev.c
		${var_proj_src_dir}/mc-fle2-rfds.c
		${var_proj_src_dir}/mc-range-edge-generation.c
		${var_proj_src_dir}/mc-range-encoding.c
		${var_proj_src_dir}/mc-range-mincover.c
		${var_proj_src_dir}/mc-rangeopts.c
		${var_proj_src_dir}/mc-reader.c
		${var_proj_src_dir}/mc-tokens.c
		${var_proj_src_dir}/mc-writer.c
		${var_proj_src_dir}/mongocrypt-binary.c
		${var_proj_src_dir}/mongocrypt-buffer.c
		${var_proj_src_dir}/mongocrypt-cache-collinfo.c
		${var_proj_src_dir}/mongocrypt-cache-key.c
		${var_proj_src_dir}/mongocrypt-cache-oauth.c
		${var_proj_src_dir}/mongocrypt-cache.c
		${var_proj_src_dir}/mongocrypt-ciphertext.c
		${var_proj_src_dir}/mongocrypt-crypto.c
		${var_proj_src_dir}/mongocrypt-ctx-datakey.c
		${var_proj_src_dir}/mongocrypt-ctx-decrypt.c
		${var_proj_src_dir}/mongocrypt-ctx-encrypt.c
		${var_proj_src_dir}/mongocrypt-ctx-rewrap-many-datakey.c
		${var_proj_src_dir}/mongocrypt-ctx.c
		${var_proj_src_dir}/mongocrypt-endpoint.c
		${var_proj_src_dir}/mongocrypt-kek.c
		${var_proj_src_dir}/mongocrypt-key-broker.c
		${var_proj_src_dir}/mongocrypt-key.c
		${var_proj_src_dir}/mongocrypt-kms-ctx.c
		${var_proj_src_dir}/mongocrypt-log.c
		${var_proj_src_dir}/mongocrypt-marking.c
		${var_proj_src_dir}/mongocrypt-opts.c
		${var_proj_src_dir}/mongocrypt-status.c
		${var_proj_src_dir}/mongocrypt-traverse-util.c
		${var_proj_src_dir}/mongocrypt-util.c
		${var_proj_src_dir}/mongocrypt.c
		${var_proj_src_dir}/os_posix/os_dll_posix.c
		${var_proj_src_dir}/os_posix/os_mutex_posix.c
		${var_proj_src_dir}/os_win/os_dll_win.c
		${var_proj_src_dir}/os_win/os_mutex_win.c
)

set(var_proj_bin_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_BIN_OUTDIR})
set(var_proj_lib_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_LIB_OUTDIR})
set(var_proj_ach_outdir ${var_proj_lib_outdir})

set(var_proj_include_dirs 
		${var_proj_src_dir}
		${var_proj_src_dir}/../include
		${USRDEF_CMAKE_INCLUDE_DIRS}
)

set(var_proj_c_defineds "-DBSON_STATIC -DBUILD_MONGODB_DRIVER_YGGR_VER -DKMS_MSG_STATIC -DMLIB_USER -DMONGOCRYPT_INTELDFP -DMONGOCRYPT_LITTLE_ENDIAN -DMONGOCRYPT_STATIC_DEFINE -DMONGOC_COMPILATION -DMONGOC_STATIC ${USRDEF_CMAKE_DEFINEDS}")

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

