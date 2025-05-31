#
#	yggr_devel definitions.
#

APP_ABI := all
#APP_ABI := arm64-v8a
#APP_ABI := armeabi-v7a
#APP_ABI := x86
#APP_ABI := x86_64
#APP_ABI := riscv64
APP_STL := c++_static

include $(CLEAR_VARS)

ifeq ($(NDK_DEBUG), 1)
	APP_DEBUG := True
	APP_OPTIM := debug
else
	APP_DEBUG := False
	APP_OPTIM := release
endif

USRDEF_APP_MIN_SDK_VERSION := 21
USRDEF_APP_RISCV64_MIN_SDK_VERSION := 35
USRDEF_APP_COMPILER_VERSION := clang
USRDEF_APP_COMPILER_VERSION_TAG := -clang

USRDEF_APP_BOOST_VERSION := 1_82
USRDEF_APP_BOOST_VERSION_TAG := -1_82

USRDEF_APP_PYTHON_VERSION := 312

USRDEF_APP_C_INCLUDE := \
	$(NDK_ROOT)/../../../../ndk_third_part_local/unixODBC/include \
	$(NDK_ROOT)/../../../../ndk_third_part_local/ncurses/include \
	$(NDK_ROOT)/../../../../ndk_third_part_local/readline/include \
	$(NDK_ROOT)/../../../../ndk_third_part_local/base64/include \
	$(NDK_ROOT)/../../../../ndk_third_part_local/blowfish/include \
	$(NDK_ROOT)/../../../../ndk_third_part_local/md5/include \
	$(NDK_ROOT)/../../../../ndk_third_part_local/sha/include \
	$(NDK_ROOT)/../../../../ndk_third_part_local/libiconv/include \
	$(NDK_ROOT)/../../../../ndk_third_part_local/libicu/include \
	$(NDK_ROOT)/../../../../ndk_third_part_local/LzmaLib/include \
	$(NDK_ROOT)/../../../../ndk_third_part_local/openssl/include \
	$(NDK_ROOT)/../../../../ndk_third_part_local/python/include \
	$(NDK_ROOT)/../../../../ndk_third_part_local/dtl/include \
	$(NDK_ROOT)/../../../../ndk_third_part_local/mongodb_c_dirver/include \
	$(NDK_ROOT)/../../../../ndk_third_part_local/mongodb_c_dirver/include/bson \
	$(NDK_ROOT)/../../../../ndk_third_part_local/mongodb_c_dirver/include/common \
	$(NDK_ROOT)/../../../../ndk_third_part_local/mongodb_c_dirver/include/jsonsl \
	$(NDK_ROOT)/../../../../ndk_third_part_local/mongodb_c_dirver/include/kms_message \
	$(NDK_ROOT)/../../../../ndk_third_part_local/mongodb_c_dirver/include/mongo_crypt \
	$(NDK_ROOT)/../../../../ndk_third_part_local/mongodb_c_dirver/include/mongoc \
	$(NDK_ROOT)/../../../../ndk_third_part_local/mongodb_c_dirver/include/mongocrypt \
	$(NDK_ROOT)/../../../../ndk_third_part_local/mongodb_c_dirver/include/mongocrypt/unicode \
	$(NDK_ROOT)/../../../../ndk_third_part_local/mongodb_c_dirver/include/utf8proc \
	$(NDK_ROOT)/../../../../ndk_third_part_local/boost/include \
	$(NDK_ROOT)/../../../../ndk_third_part_local/lua/include \
	$(NDK_ROOT)/../../../../ndk_third_part_local/luabind/include 

USRDEF_OUTDIR_ROOT := ./stage
USRDEF_NDK_APP_OUT := $(USRDEF_OUTDIR_ROOT)/objs
USRDEF_PROJ_OUTDIR := $(USRDEF_OUTDIR_ROOT)/yggr

USRDEF_APP_CFLAGS := 
USRDEF_APP_CFLAGS_DEBUG := 
USRDEF_APP_CFLAGS_RELEASE := 

USRDEF_APP_CPPFLAGS := 
USRDEF_APP_CPPFLAGS_DEBUG := 
USRDEF_APP_CPPFLAGS_RELEASE := 

USRDEF_APP_LD_LIBRARY_PATH := \
	$(NDK_ROOT)/../../../../ndk_third_part_local/unixODBC/lib \
	$(NDK_ROOT)/../../../../ndk_third_part_local/ncurses/lib \
	$(NDK_ROOT)/../../../../ndk_third_part_local/readline/lib \
	$(NDK_ROOT)/../../../../ndk_third_part_local/base64/lib \
	$(NDK_ROOT)/../../../../ndk_third_part_local/blowfish/lib \
	$(NDK_ROOT)/../../../../ndk_third_part_local/md5/lib \
	$(NDK_ROOT)/../../../../ndk_third_part_local/sha/lib \
	$(NDK_ROOT)/../../../../ndk_third_part_local/libiconv/lib \
	$(NDK_ROOT)/../../../../ndk_third_part_local/libicu/lib \
	$(NDK_ROOT)/../../../../ndk_third_part_local/LzmaLib/lib \
	$(NDK_ROOT)/../../../../ndk_third_part_local/openssl/lib \
	$(NDK_ROOT)/../../../../ndk_third_part_local/python/lib \
	$(NDK_ROOT)/../../../../ndk_third_part_local/dtl/lib \
	$(NDK_ROOT)/../../../../ndk_third_part_local/mongodb_c_dirver/lib \
	$(NDK_ROOT)/../../../../ndk_third_part_local/boost/lib \
	$(NDK_ROOT)/../../../../ndk_third_part_local/lua/lib \
	$(NDK_ROOT)/../../../../ndk_third_part_local/luabind/lib 

USRDEF_APP_LDFLAGS_DEBUG := 
USRDEF_APP_LDFLAGS_RELEASE := 
USRDEF_APP_LDFLAGS := 

NDK_APP_OUT := $(USRDEF_NDK_APP_OUT)

APP_CPPFLAGS := -std=c++11
APP_PLATFORM := android-$(USRDEF_APP_MIN_SDK_VERSION)
