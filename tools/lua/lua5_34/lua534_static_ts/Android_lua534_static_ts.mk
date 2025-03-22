#
#	lua534_static_ts definitions.
#

LOCAL_PATH := $(call my-dir)

USRDEF_LOCAL_PROJECT_NAME := lua534_static_ts

l_path := $(LOCAL_PATH)

# output module name
ifeq ($(NDK_DEBUG), 1)
	l_module := liblua503_ts$(USRDEF_APP_COMPILER_VERSION_TAG)-s-d
else
	l_module := liblua503_ts$(USRDEF_APP_COMPILER_VERSION_TAG)-s
endif

# source files
l_src_file := \
	../lua534/lapi.c \
	../lua534/lauxlib.c \
	../lua534/lbaselib.c \
	../lua534/lbitlib.c \
	../lua534/lcode.c \
	../lua534/lcorolib.c \
	../lua534/lctype.c \
	../lua534/ldblib.c \
	../lua534/ldebug.c \
	../lua534/ldo.c \
	../lua534/ldump.c \
	../lua534/lfunc.c \
	../lua534/lgc.c \
	../lua534/linit.c \
	../lua534/liolib.c \
	../lua534/llex.c \
	../lua534/lmathlib.c \
	../lua534/lmem.c \
	../lua534/loadlib.c \
	../lua534/lobject.c \
	../lua534/lopcodes.c \
	../lua534/loslib.c \
	../lua534/lparser.c \
	../lua534/lstate.c \
	../lua534/lstring.c \
	../lua534/lstrlib.c \
	../lua534/ltable.c \
	../lua534/ltablib.c \
	../lua534/ltm.c \
	../lua534/lundump.c \
	../lua534/lutf8lib.c \
	../lua534/lvm.c \
	../lua534/lzio.c \

# include path
l_c_include := \
	$(l_path) \
	$(l_path)/../include \
	$(USRDEF_APP_C_INCLUDE) \

# cflags
l_cflags := \
	-DANDROID \
	-DLUA_COMPAT_5_1 \
	-DLUA_COMPAT_5_2 \
	-DLUA_USE_LINUX \
	-DYGGR_LUA_THREAD_SAFE \
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
