#
#	bson definitions.
#

LOCAL_PATH := $(call my-dir)

USRDEF_LOCAL_PROJECT_NAME := bson

l_path := $(LOCAL_PATH)

# output module name
ifeq ($(NDK_DEBUG), 1)
	l_module := lib$(USRDEF_LOCAL_PROJECT_NAME)$(USRDEF_APP_COMPILER_VERSION_TAG)-d
else
	l_module := lib$(USRDEF_LOCAL_PROJECT_NAME)$(USRDEF_APP_COMPILER_VERSION_TAG)
endif

# source files
l_src_file := \
	bcon.c \
	bson-atomic.c \
	bson-clock.c \
	bson-context.c \
	bson-decimal128.c \
	bson-error.c \
	bson-iso8601.c \
	bson-iter.c \
	bson-json.c \
	bson-keys.c \
	bson-md5.c \
	bson-memory.c \
	bson-oid.c \
	bson-reader.c \
	bson-string.c \
	bson-timegm.c \
	bson-utf8.c \
	bson-value.c \
	bson-version-functions.c \
	bson-writer.c \
	bson.c \
	jsonsl.c \

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
	-DBSON_COMPILATION \
	-DBSON_STATIC \
	-DBUILD_MONGODB_DRIVER_YGGR_VER \
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
		-DKMS_MSG_STATIC \
		-DMLIB_USER \
		-DMONGOCRYPT_LITTLE_ENDIAN \
		-DMONGOCRYPT_STATIC_DEFINE \
		-DMONGOC_STATIC \
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
