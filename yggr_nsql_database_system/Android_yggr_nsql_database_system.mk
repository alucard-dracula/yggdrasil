#
#	yggr_nsql_database_system definitions.
#

LOCAL_PATH := $(call my-dir)

USRDEF_LOCAL_PROJECT_NAME := yggr_nsql_database_system

l_path := $(LOCAL_PATH)

# output module name
ifeq ($(NDK_DEBUG), 1)
	l_module := lib$(USRDEF_LOCAL_PROJECT_NAME)$(USRDEF_APP_COMPILER_VERSION_TAG)-d
else
	l_module := lib$(USRDEF_LOCAL_PROJECT_NAME)$(USRDEF_APP_COMPILER_VERSION_TAG)
endif

# source files
l_src_file := \
	bson_date_time_native_ex.cpp \
	bson_decimal128_native_ex.cpp \
	bson_error_native_ex.cpp \
	bson_iterator_native_ex.cpp \
	bson_native_ex.cpp \
	bson_oid_native_ex.cpp \
	bson_regex_native_ex.cpp \
	bson_string_native_ex.cpp \
	bson_timeval_native_ex.cpp \
	bson_value_conflict_fixer.cpp \
	bson_value_native_ex.cpp \
	c_bson.cpp \
	c_bson_code.cpp \
	c_bson_date.cpp \
	c_bson_decimal128.cpp \
	c_bson_error.cpp \
	c_bson_oid.cpp \
	c_bson_regex.cpp \
	c_bson_string_item.cpp \
	c_bson_symbol.cpp \
	c_bson_time.cpp \
	c_bson_timestamp.cpp \
	c_bson_timeval.cpp \
	c_bson_value.cpp \
	c_mongo_apm_callbacks.cpp \
	c_mongo_auto_encryption_opts.cpp \
	c_mongo_basic_stream.cpp \
	c_mongo_buffered_stream.cpp \
	c_mongo_client_encryption_datakey_opts.cpp \
	c_mongo_client_encryption_encrypt_opts.cpp \
	c_mongo_client_encryption_encrypt_range_opts.cpp \
	c_mongo_client_encryption_opts.cpp \
	c_mongo_client_encryption_rewrap_many_datakey_result.cpp \
	c_mongo_connection.cpp \
	c_mongo_connection_encryption.cpp \
	c_mongo_connection_init.cpp \
	c_mongo_connection_pool.cpp \
	c_mongo_enc_connection.cpp \
	c_mongo_file_stream.cpp \
	c_mongo_find_and_modify_opts.cpp \
	c_mongo_gridfs_download_stream.cpp \
	c_mongo_gridfs_file.cpp \
	c_mongo_gridfs_file_opt.cpp \
	c_mongo_gridfs_stream.cpp \
	c_mongo_gridfs_upload_stream.cpp \
	c_mongo_host_info.cpp \
	c_mongo_index_description.cpp \
	c_mongo_index_model.cpp \
	c_mongo_index_opt_geo.cpp \
	c_mongo_index_opt_wt.cpp \
	c_mongo_iovec.cpp \
	c_mongo_mc_kms_credentials_callback.cpp \
	c_mongo_read_concern.cpp \
	c_mongo_read_prefs.cpp \
	c_mongo_server_api.cpp \
	c_mongo_server_description.cpp \
	c_mongo_socket_stream.cpp \
	c_mongo_ssl_opt.cpp \
	c_mongo_tls_stream.cpp \
	c_mongo_uri.cpp \
	c_mongo_write_concern.cpp \
	mongoc_auto_encryption_opts_native_ex.cpp \
	mongoc_client_encryption_datakey_opts_native_ex.cpp \
	mongoc_client_encryption_encrypt_opts_native_ex.cpp \
	mongoc_client_encryption_encrypt_range_opts_native_ex.cpp \
	mongoc_client_encryption_opts_native_ex.cpp \
	mongoc_client_native_ex.cpp \
	mongoc_collection_native_ex.cpp \
	mongoc_find_and_modify_opts_native_ex.cpp \
	mongoc_gridfs_bucket_file_t_native_ex.cpp \
	mongoc_gridfs_bucket_native_ex.cpp \
	mongoc_gridfs_bucket_t_native_ex.cpp \
	mongoc_gridfs_file_native_ex.cpp \
	mongoc_gridfs_native_ex.cpp \
	mongoc_host_list_native_ex.cpp \
	mongoc_read_concern_native_ex.cpp \
	mongoc_read_prefs_native_ex.cpp \
	mongoc_server_description_native_ex.cpp \
	mongoc_uri_native_ex.cpp \
	mongoc_write_concern_native_ex.cpp \
	mongodb_installer.cpp \

# include path
l_c_include := \
	$(l_path) \
	$(l_path)/../ \
	$(USRDEF_APP_C_INCLUDE) \

# cflags
l_cflags := \
	-DANDROID \
	-D__ANDROID__ \
	-Wall \
	-Werror=endif-labels \
	-Werror=reorder \
	-Werror=return-type \
	-Wno-comment \
	-Wno-enum-compare \
	-Wno-multichar \
	-Wno-sign-compare \
	-Wno-unused-local-typedefs \
	-fPIC \
	-fdata-sections \
	-fexceptions \
	-ffunction-sections \
	-fno-strict-aliasing \
	-ftemplate-backtrace-limit=0 \
	$(USRDEF_APP_CFLAGS) \

ifeq ($(NDK_DEBUG), 1)
	l_cflags += \
		-D_DEBUG \
		-O0 \
		-g \
		$(USRDEF_APP_CFLAGS_DEBUG) \

else
	l_cflags += \
		-DMONGO_STATIC_BUILD \
		-DNDEBUG \
		-O3 \
		$(USRDEF_APP_CFLAGS_RELEASE) \

endif

# 64bit cflag
ifeq ("$(TARGET_ARCH_ABI)", "arm64-v8a")
	l_cflags += -DYGGR_SYSTEM_64
endif 

ifeq ("$(TARGET_ARCH_ABI)", "x86_64")
	l_cflags += -DYGGR_SYSTEM_64
endif 

ifeq ("$(TARGET_ARCH_ABI)", "riscv64")
	l_cflags += -DYGGR_SYSTEM_64
endif 

# cppflags
l_cppflags := \
	-frtti \
	$(USRDEF_APP_CPPFLAGS) \

ifeq ($(NDK_DEBUG), 1)
	l_cppflags += \
		-fno-elide-constructors \
		$(USRDEF_APP_CPPFLAGS_DEBUG) \

else
	l_cppflags += \
		$(USRDEF_APP_CPPFLAGS_RELEASE) \

endif

# ldflags
ifeq ($(NDK_DEBUG), 1)
	l_ldflags := \
		$(USRDEF_APP_LDFLAGS_DEBUG) \

else
	l_ldflags := \
		$(USRDEF_APP_LDFLAGS_RELEASE)\

endif

l_ldflags += \
	$(USRDEF_APP_LDFLAGS) \


# build it

include $(CLEAR_VARS)

LOCAL_MODULE := $(l_module)
LOCAL_SRC_FILES := $(l_src_file)
LOCAL_C_INCLUDES := $(l_c_include)
LOCAL_CFLAGS := $(l_cflags)
LOCAL_CPPFLAGS := $(l_cppflags)
#LOCAL_LDFLAGS := $(l_ldflags)

include $(BUILD_STATIC_LIBRARY)
