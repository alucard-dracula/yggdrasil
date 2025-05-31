#yggr_nsql_database_system CMakeLists.txt

project(yggr_nsql_database_system)

message(STATUS "build-sub-project [ yggr_nsql_database_system ]")

set(var_proj_name yggr_nsql_database_system)

set(var_root_proj_dir ${PROJECT_SOURCE_DIR}/../../..)
set(var_proj_src_dir ${var_root_proj_dir}/yggr_nsql_database_system)

set(var_proj_src_list 
		${var_proj_src_dir}/bson_date_time_native_ex.cpp
		${var_proj_src_dir}/bson_decimal128_native_ex.cpp
		${var_proj_src_dir}/bson_error_native_ex.cpp
		${var_proj_src_dir}/bson_iterator_native_ex.cpp
		${var_proj_src_dir}/bson_native_ex.cpp
		${var_proj_src_dir}/bson_oid_native_ex.cpp
		${var_proj_src_dir}/bson_regex_native_ex.cpp
		${var_proj_src_dir}/bson_string_native_ex.cpp
		${var_proj_src_dir}/bson_timeval_native_ex.cpp
		${var_proj_src_dir}/bson_value_conflict_fixer.cpp
		${var_proj_src_dir}/bson_value_native_ex.cpp
		${var_proj_src_dir}/c_bson.cpp
		${var_proj_src_dir}/c_bson_code.cpp
		${var_proj_src_dir}/c_bson_date.cpp
		${var_proj_src_dir}/c_bson_decimal128.cpp
		${var_proj_src_dir}/c_bson_error.cpp
		${var_proj_src_dir}/c_bson_oid.cpp
		${var_proj_src_dir}/c_bson_regex.cpp
		${var_proj_src_dir}/c_bson_string_item.cpp
		${var_proj_src_dir}/c_bson_symbol.cpp
		${var_proj_src_dir}/c_bson_time.cpp
		${var_proj_src_dir}/c_bson_timestamp.cpp
		${var_proj_src_dir}/c_bson_timeval.cpp
		${var_proj_src_dir}/c_bson_value.cpp
		${var_proj_src_dir}/c_mongo_apm_callbacks.cpp
		${var_proj_src_dir}/c_mongo_auto_encryption_opts.cpp
		${var_proj_src_dir}/c_mongo_basic_stream.cpp
		${var_proj_src_dir}/c_mongo_buffered_stream.cpp
		${var_proj_src_dir}/c_mongo_client_encryption_datakey_opts.cpp
		${var_proj_src_dir}/c_mongo_client_encryption_encrypt_opts.cpp
		${var_proj_src_dir}/c_mongo_client_encryption_encrypt_range_opts.cpp
		${var_proj_src_dir}/c_mongo_client_encryption_opts.cpp
		${var_proj_src_dir}/c_mongo_client_encryption_rewrap_many_datakey_result.cpp
		${var_proj_src_dir}/c_mongo_connection.cpp
		${var_proj_src_dir}/c_mongo_connection_encryption.cpp
		${var_proj_src_dir}/c_mongo_connection_init.cpp
		${var_proj_src_dir}/c_mongo_connection_pool.cpp
		${var_proj_src_dir}/c_mongo_enc_connection.cpp
		${var_proj_src_dir}/c_mongo_file_stream.cpp
		${var_proj_src_dir}/c_mongo_find_and_modify_opts.cpp
		${var_proj_src_dir}/c_mongo_gridfs_download_stream.cpp
		${var_proj_src_dir}/c_mongo_gridfs_file.cpp
		${var_proj_src_dir}/c_mongo_gridfs_file_opt.cpp
		${var_proj_src_dir}/c_mongo_gridfs_stream.cpp
		${var_proj_src_dir}/c_mongo_gridfs_upload_stream.cpp
		${var_proj_src_dir}/c_mongo_host_info.cpp
		${var_proj_src_dir}/c_mongo_index_description.cpp
		${var_proj_src_dir}/c_mongo_index_model.cpp
		${var_proj_src_dir}/c_mongo_index_opt_geo.cpp
		${var_proj_src_dir}/c_mongo_index_opt_wt.cpp
		${var_proj_src_dir}/c_mongo_iovec.cpp
		${var_proj_src_dir}/c_mongo_mc_kms_credentials_callback.cpp
		${var_proj_src_dir}/c_mongo_read_concern.cpp
		${var_proj_src_dir}/c_mongo_read_prefs.cpp
		${var_proj_src_dir}/c_mongo_server_api.cpp
		${var_proj_src_dir}/c_mongo_server_description.cpp
		${var_proj_src_dir}/c_mongo_socket_stream.cpp
		${var_proj_src_dir}/c_mongo_ssl_opt.cpp
		${var_proj_src_dir}/c_mongo_tls_stream.cpp
		${var_proj_src_dir}/c_mongo_uri.cpp
		${var_proj_src_dir}/c_mongo_write_concern.cpp
		${var_proj_src_dir}/mongoc_auto_encryption_opts_native_ex.cpp
		${var_proj_src_dir}/mongoc_client_encryption_datakey_opts_native_ex.cpp
		${var_proj_src_dir}/mongoc_client_encryption_encrypt_opts_native_ex.cpp
		${var_proj_src_dir}/mongoc_client_encryption_encrypt_range_opts_native_ex.cpp
		${var_proj_src_dir}/mongoc_client_encryption_opts_native_ex.cpp
		${var_proj_src_dir}/mongoc_client_native_ex.cpp
		${var_proj_src_dir}/mongoc_collection_native_ex.cpp
		${var_proj_src_dir}/mongoc_find_and_modify_opts_native_ex.cpp
		${var_proj_src_dir}/mongoc_gridfs_bucket_file_t_native_ex.cpp
		${var_proj_src_dir}/mongoc_gridfs_bucket_native_ex.cpp
		${var_proj_src_dir}/mongoc_gridfs_bucket_t_native_ex.cpp
		${var_proj_src_dir}/mongoc_gridfs_file_native_ex.cpp
		${var_proj_src_dir}/mongoc_gridfs_native_ex.cpp
		${var_proj_src_dir}/mongoc_host_list_native_ex.cpp
		${var_proj_src_dir}/mongoc_read_concern_native_ex.cpp
		${var_proj_src_dir}/mongoc_read_prefs_native_ex.cpp
		${var_proj_src_dir}/mongoc_server_description_native_ex.cpp
		${var_proj_src_dir}/mongoc_uri_native_ex.cpp
		${var_proj_src_dir}/mongoc_write_concern_native_ex.cpp
		${var_proj_src_dir}/mongodb_installer.cpp
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

set(var_proj_c_defineds_release "-DMONGO_STATIC_BUILD -DNDEBUG -DYGGR_SYSTEM_64 ${USRDEF_CMAKE_DEFINEDS_RELEASE}")

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

