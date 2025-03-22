#
#	lua514_static_ts definitions.
#

LOCAL_PATH := $(call my-dir)

USRDEF_LOCAL_PROJECT_NAME := lua514_static_ts

l_path := $(LOCAL_PATH)

# output module name
ifeq ($(NDK_DEBUG), 1)
	l_module := liblua501_ts$(USRDEF_APP_COMPILER_VERSION_TAG)-s-d
else
	l_module := liblua501_ts$(USRDEF_APP_COMPILER_VERSION_TAG)-s
endif

# source files
l_src_file := \
	../lua514/lapi.c \
	../lua514/lauxlib.c \
	../lua514/lbaselib.c \
	../lua514/lcode.c \
	../lua514/ldblib.c \
	../lua514/ldebug.c \
	../lua514/ldo.c \
	../lua514/ldump.c \
	../lua514/lfunc.c \
	../lua514/lgc.c \
	../lua514/linit.c \
	../lua514/liolib.c \
	../lua514/llex.c \
	../lua514/lmathlib.c \
	../lua514/lmem.c \
	../lua514/loadlib.c \
	../lua514/lobject.c \
	../lua514/lopcodes.c \
	../lua514/loslib.c \
	../lua514/lparser.c \
	../lua514/lstate.c \
	../lua514/lstring.c \
	../lua514/lstrlib.c \
	../lua514/ltable.c \
	../lua514/ltablib.c \
	../lua514/ltm.c \
	../lua514/lundump.c \
	../lua514/lvm.c \
	../lua514/lzio.c \
	../lua514/print.c \

# include path
l_c_include := \
	$(l_path) \
	$(l_path)/../include \
	$(USRDEF_APP_C_INCLUDE) \

# cflags
l_cflags := \
	-DANDROID \
	-DLUA_USELONGLONG \
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
