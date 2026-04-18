#
#	lua546_ts definitions.
#

LOCAL_PATH := $(call my-dir)

USRDEF_LOCAL_PROJECT_NAME := lua546_ts

l_path := $(LOCAL_PATH)

# output module name
ifeq ($(NDK_DEBUG), 1)
	l_module := lua504_ts$(USRDEF_APP_COMPILER_VERSION_TAG)-d
else
	l_module := lua504_ts$(USRDEF_APP_COMPILER_VERSION_TAG)
endif

# source files
l_src_file := \
	../lua546/lapi.c \
	../lua546/lauxlib.c \
	../lua546/lbaselib.c \
	../lua546/lcode.c \
	../lua546/lcorolib.c \
	../lua546/lctype.c \
	../lua546/ldblib.c \
	../lua546/ldebug.c \
	../lua546/ldo.c \
	../lua546/ldump.c \
	../lua546/lfunc.c \
	../lua546/lgc.c \
	../lua546/linit.c \
	../lua546/liolib.c \
	../lua546/llex.c \
	../lua546/lmathlib.c \
	../lua546/lmem.c \
	../lua546/loadlib.c \
	../lua546/lobject.c \
	../lua546/lopcodes.c \
	../lua546/loslib.c \
	../lua546/lparser.c \
	../lua546/lstate.c \
	../lua546/lstring.c \
	../lua546/lstrlib.c \
	../lua546/ltable.c \
	../lua546/ltablib.c \
	../lua546/ltm.c \
	../lua546/lundump.c \
	../lua546/lutf8lib.c \
	../lua546/lvm.c \
	../lua546/lzio.c \

# include path
l_c_include := \
	$(l_path) \
	$(l_path)/../include \
	$(USRDEF_APP_C_INCLUDE) \

# cflags
l_cflags := \
	-DANDROID \
	-DBUILD_DLL \
	-DLUA546_TS_EXPORT \
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
ifeq ("$(TARGET_ARCH_ABI)", "arm64-v8a")
	USEDEF_CLANG_ABI := aarch64-linux-android
	tag_arch_bits := -a64
endif

ifeq ("$(TARGET_ARCH_ABI)", "armeabi-v7a")
	USEDEF_CLANG_ABI := armv7a-linux-androideabi
	tag_arch_bits := -a32
endif

ifeq ("$(TARGET_ARCH_ABI)", "riscv64")
	USEDEF_CLANG_ABI := riscv64-linux-android
	tag_arch_bits := -r64
endif

ifeq ("$(TARGET_ARCH_ABI)", "x86_64")
	USEDEF_CLANG_ABI := x86_64-linux-android
	tag_arch_bits := 
endif

ifeq ("$(TARGET_ARCH_ABI)", "x86")
	USEDEF_CLANG_ABI := i686-linux-android
	tag_arch_bits := 
endif

ifeq ("$(TARGET_ARCH_ABI)", "riscv64")
	l_ld_lib_path := \
		$(addprefix -L, $(addsuffix /$(USEDEF_CLANG_ABI)/$(USRDEF_APP_RISCV64_MIN_SDK_VERSION), \
		$(USRDEF_APP_LD_LIBRARY_PATH) \
		))
else
	l_ld_lib_path := \
		$(addprefix -L, $(addsuffix /$(USEDEF_CLANG_ABI)/$(USRDEF_APP_MIN_SDK_VERSION), \
		$(USRDEF_APP_LD_LIBRARY_PATH) \
		))
endif

l_ldflags := \
	$(l_ld_lib_path) \

ifeq ($(NDK_DEBUG), 1)
	l_ldflags += \
		$(USRDEF_APP_LDFLAGS_DEBUG) \

else
	l_ldflags += \
		-s \
		$(USRDEF_APP_LDFLAGS_RELEASE) \

endif

l_ldflags += \
	$(USRDEF_APP_LDFLAGS)\

# out_dir

l_app_dst := $(USRDEF_PROJ_OUTDIR)/lib/$(TARGET_ARCH_ABI)

# build it

include $(CLEAR_VARS)

LOCAL_MODULE := $(l_module)
LOCAL_SRC_FILES := $(l_src_file)
LOCAL_C_INCLUDES := $(l_c_include)
LOCAL_CFLAGS := $(l_cflags)
LOCAL_CPPFLAGS := $(l_cppflags)
LOCAL_LDFLAGS := $(l_ldflags)

NDK_APP_DST_DIR := $(l_app_dst)

include $(BUILD_SHARED_LIBRARY)
