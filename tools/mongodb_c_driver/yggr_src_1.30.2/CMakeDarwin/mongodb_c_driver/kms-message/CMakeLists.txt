#kms-message CMakeLists.txt

project(kms-message)

message(STATUS "build-sub-project [ kms-message ]")

set(var_proj_name kms-message)

set(var_root_proj_dir ${PROJECT_SOURCE_DIR}/../../..)
set(var_proj_src_dir ${var_root_proj_dir}/kms-message)

set(var_proj_src_list 
		${var_proj_src_dir}/hexlify.c
		${var_proj_src_dir}/kms_azure_request.c
		${var_proj_src_dir}/kms_b64.c
		${var_proj_src_dir}/kms_caller_identity_request.c
		${var_proj_src_dir}/kms_crypto_apple.c
		${var_proj_src_dir}/kms_crypto_libcrypto.c
		${var_proj_src_dir}/kms_crypto_none.c
		${var_proj_src_dir}/kms_crypto_windows.c
		${var_proj_src_dir}/kms_decrypt_request.c
		${var_proj_src_dir}/kms_encrypt_request.c
		${var_proj_src_dir}/kms_gcp_request.c
		${var_proj_src_dir}/kms_kmip_reader_writer.c
		${var_proj_src_dir}/kms_kmip_request.c
		${var_proj_src_dir}/kms_kmip_response.c
		${var_proj_src_dir}/kms_kmip_response_parser.c
		${var_proj_src_dir}/kms_kv_list.c
		${var_proj_src_dir}/kms_message.c
		${var_proj_src_dir}/kms_port.c
		${var_proj_src_dir}/kms_request.c
		${var_proj_src_dir}/kms_request_opt.c
		${var_proj_src_dir}/kms_request_str.c
		${var_proj_src_dir}/kms_response.c
		${var_proj_src_dir}/kms_response_parser.c
		${var_proj_src_dir}/sort.c
)

set(var_proj_bin_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_BIN_OUTDIR})
set(var_proj_lib_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_LIB_OUTDIR})
set(var_proj_ach_outdir ${var_proj_lib_outdir})

set(var_proj_include_dirs 
		${var_proj_src_dir}
		${var_proj_src_dir}/../include
		${var_proj_src_dir}/../include/bson
		${var_proj_src_dir}/../include/common
		${var_proj_src_dir}/../include/jsonsl
		${var_proj_src_dir}/../include/kms_message
		${var_proj_src_dir}/../include/mongo_crypt
		${var_proj_src_dir}/../include/mongoc
		${var_proj_src_dir}/../include/mongocrypt
		${var_proj_src_dir}/../include/mongocrypt/unicode
		${var_proj_src_dir}/../include/utf8proc
		${USRDEF_CMAKE_INCLUDE_DIRS}
)

set(var_proj_c_defineds "-DBSON_COMPILATION -DBSON_STATIC -DBUILD_MONGODB_DRIVER_YGGR_VER -DKMS_MSG_STATIC -DMLIB_USER -DMONGOCRYPT_LITTLE_ENDIAN -DMONGOCRYPT_STATIC_DEFINE -DMONGOC_STATIC ${USRDEF_CMAKE_DEFINEDS}")

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

