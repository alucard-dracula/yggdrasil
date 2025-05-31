#
#	mongoc definitions.
#

LOCAL_PATH := $(call my-dir)

USRDEF_LOCAL_PROJECT_NAME := mongoc

l_path := $(LOCAL_PATH)

# output module name
ifeq ($(NDK_DEBUG), 1)
	l_module := lib$(USRDEF_LOCAL_PROJECT_NAME)$(USRDEF_APP_COMPILER_VERSION_TAG)-d
else
	l_module := lib$(USRDEF_LOCAL_PROJECT_NAME)$(USRDEF_APP_COMPILER_VERSION_TAG)
endif

# source files
l_src_file := \
	mcd-azure.c \
	mcd-rpc.c \
	mongoc-aggregate.c \
	mongoc-apm.c \
	mongoc-array.c \
	mongoc-async-cmd.c \
	mongoc-async.c \
	mongoc-buffer.c \
	mongoc-bulk-operation.c \
	mongoc-change-stream.c \
	mongoc-client-pool.c \
	mongoc-client-session.c \
	mongoc-client-side-encryption.c \
	mongoc-client.c \
	mongoc-cluster-aws.c \
	mongoc-cluster-cyrus.c \
	mongoc-cluster-sasl.c \
	mongoc-cluster-sspi.c \
	mongoc-cluster.c \
	mongoc-cmd.c \
	mongoc-collection.c \
	mongoc-compression.c \
	mongoc-counters.c \
	mongoc-crypt.c \
	mongoc-crypto-cng.c \
	mongoc-crypto-common-crypto.c \
	mongoc-crypto-openssl.c \
	mongoc-crypto.c \
	mongoc-cursor-array.c \
	mongoc-cursor-change-stream.c \
	mongoc-cursor-cmd-deprecated.c \
	mongoc-cursor-cmd.c \
	mongoc-cursor-find-cmd.c \
	mongoc-cursor-find-opquery.c \
	mongoc-cursor-find.c \
	mongoc-cursor-legacy.c \
	mongoc-cursor.c \
	mongoc-cyrus.c \
	mongoc-database.c \
	mongoc-deprioritized-servers.c \
	mongoc-error.c \
	mongoc-find-and-modify.c \
	mongoc-flags.c \
	mongoc-generation-map.c \
	mongoc-gridfs-bucket-file.c \
	mongoc-gridfs-bucket.c \
	mongoc-gridfs-file-list.c \
	mongoc-gridfs-file-page.c \
	mongoc-gridfs-file.c \
	mongoc-gridfs.c \
	mongoc-handshake.c \
	mongoc-host-list.c \
	mongoc-http.c \
	mongoc-index.c \
	mongoc-init.c \
	mongoc-interrupt.c \
	mongoc-libressl.c \
	mongoc-linux-distro-scanner.c \
	mongoc-list.c \
	mongoc-log.c \
	mongoc-matcher-op.c \
	mongoc-matcher.c \
	mongoc-memcmp.c \
	mongoc-ocsp-cache.c \
	mongoc-opcode.c \
	mongoc-openssl.c \
	mongoc-optional.c \
	mongoc-opts-helpers.c \
	mongoc-opts.c \
	mongoc-queue.c \
	mongoc-rand-cng.c \
	mongoc-rand-common-crypto.c \
	mongoc-rand-openssl.c \
	mongoc-read-concern.c \
	mongoc-read-prefs.c \
	mongoc-rpc.c \
	mongoc-sasl.c \
	mongoc-scram.c \
	mongoc-secure-channel.c \
	mongoc-secure-transport.c \
	mongoc-server-api.c \
	mongoc-server-description.c \
	mongoc-server-monitor.c \
	mongoc-server-stream.c \
	mongoc-set.c \
	mongoc-shared.c \
	mongoc-socket.c \
	mongoc-ssl.c \
	mongoc-sspi.c \
	mongoc-stream-buffered.c \
	mongoc-stream-file.c \
	mongoc-stream-gridfs-download.c \
	mongoc-stream-gridfs-upload.c \
	mongoc-stream-gridfs.c \
	mongoc-stream-socket.c \
	mongoc-stream-tls-libressl.c \
	mongoc-stream-tls-openssl-bio.c \
	mongoc-stream-tls-openssl.c \
	mongoc-stream-tls-secure-channel.c \
	mongoc-stream-tls-secure-transport.c \
	mongoc-stream-tls.c \
	mongoc-stream.c \
	mongoc-timeout.c \
	mongoc-topology-background-monitoring.c \
	mongoc-topology-description-apm.c \
	mongoc-topology-description.c \
	mongoc-topology-scanner.c \
	mongoc-topology.c \
	mongoc-ts-pool.c \
	mongoc-uri.c \
	mongoc-util.c \
	mongoc-version-functions.c \
	mongoc-write-command.c \
	mongoc-write-concern.c \
	service-gcp.c \

# include path
l_c_include := \
	$(l_path) \
	$(l_path)/../include \
	$(l_path)/../include/bson \
	$(l_path)/../include/common \
	$(l_path)/../include/jsonsl \
	$(l_path)/../include/kms_message \
	$(l_path)/../include/mongo_crypt \
	$(l_path)/../include/mongoc \
	$(l_path)/../include/mongocrypt \
	$(l_path)/../include/mongocrypt/unicode \
	$(l_path)/../include/utf8proc \
	$(USRDEF_APP_C_INCLUDE) \

# cflags
l_cflags := \
	-DANDROID \
	-DBSON_STATIC \
	-DBUILD_MONGODB_DRIVER_YGGR_VER \
	-DKMS_MSG_STATIC \
	-DMLIB_USER \
	-DMONGOCRYPT_LITTLE_ENDIAN \
	-DMONGOCRYPT_STATIC_DEFINE \
	-DMONGOC_COMPILATION \
	-DMONGOC_STATIC \
	-DUTF8PROC_STATIC \
	-D__ANDROID__ \
	-Wall \
	-Werror=endif-labels \
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
	$(USRDEF_APP_CFLAGS) \

ifeq ($(NDK_DEBUG), 1)
	l_cflags += \
		-D_DEBUG \
		-O0 \
		-g \
		$(USRDEF_APP_CFLAGS_DEBUG) \

else
	l_cflags += \
		-DNDEBUG \
		-O3 \
		$(USRDEF_APP_CFLAGS_RELEASE) \

endif

# cppflags
l_cppflags := \
	-frtti \
	$(USRDEF_APP_CPPFLAGS) \

ifeq ($(NDK_DEBUG), 1)
	l_cppflags += \
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
