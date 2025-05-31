#
#	mongocrypt definitions.
#

LOCAL_PATH := $(call my-dir)

USRDEF_LOCAL_PROJECT_NAME := mongocrypt

l_path := $(LOCAL_PATH)

# output module name
ifeq ($(NDK_DEBUG), 1)
	l_module := lib$(USRDEF_LOCAL_PROJECT_NAME)$(USRDEF_APP_COMPILER_VERSION_TAG)-d
else
	l_module := lib$(USRDEF_LOCAL_PROJECT_NAME)$(USRDEF_APP_COMPILER_VERSION_TAG)
endif

# source files
l_src_file := \
	crypto/cng.c \
	crypto/commoncrypto.c \
	crypto/libcrypto.c \
	crypto/none.c \
	mc-array.c \
	mc-efc.c \
	mc-fle2-encryption-placeholder.c \
	mc-fle2-find-equality-payload-v2.c \
	mc-fle2-find-equality-payload.c \
	mc-fle2-find-range-payload-v2.c \
	mc-fle2-find-range-payload.c \
	mc-fle2-insert-update-payload-v2.c \
	mc-fle2-insert-update-payload.c \
	mc-fle2-payload-iev-v2.c \
	mc-fle2-payload-iev.c \
	mc-fle2-payload-uev-common.c \
	mc-fle2-payload-uev-v2.c \
	mc-fle2-payload-uev.c \
	mc-fle2-rfds.c \
	mc-fle2-tag-and-encrypted-metadata-block.c \
	mc-range-edge-generation.c \
	mc-range-encoding.c \
	mc-range-mincover.c \
	mc-rangeopts.c \
	mc-reader.c \
	mc-schema-broker.c \
	mc-str-encode-string-sets.c \
	mc-text-search-str-encode.c \
	mc-tokens.c \
	mc-writer.c \
	mongocrypt-binary.c \
	mongocrypt-buffer.c \
	mongocrypt-cache-collinfo.c \
	mongocrypt-cache-key.c \
	mongocrypt-cache-oauth.c \
	mongocrypt-cache.c \
	mongocrypt-ciphertext.c \
	mongocrypt-crypto.c \
	mongocrypt-ctx-datakey.c \
	mongocrypt-ctx-decrypt.c \
	mongocrypt-ctx-encrypt.c \
	mongocrypt-ctx-rewrap-many-datakey.c \
	mongocrypt-ctx.c \
	mongocrypt-endpoint.c \
	mongocrypt-kek.c \
	mongocrypt-key-broker.c \
	mongocrypt-key.c \
	mongocrypt-kms-ctx.c \
	mongocrypt-log.c \
	mongocrypt-marking.c \
	mongocrypt-opts.c \
	mongocrypt-status.c \
	mongocrypt-traverse-util.c \
	mongocrypt-util.c \
	mongocrypt.c \
	os_posix/os_dll_posix.c \
	os_posix/os_mutex_posix.c \
	os_win/os_dll_win.c \
	os_win/os_mutex_win.c \
	unicode/case-fold-map.c \
	unicode/diacritic-fold-map.c \
	unicode/fold.c \

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
