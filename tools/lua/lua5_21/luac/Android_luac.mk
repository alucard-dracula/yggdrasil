#
#	luac definitions.
#

LOCAL_PATH := $(call my-dir)

USRDEF_LOCAL_PROJECT_NAME := luac

l_path := $(LOCAL_PATH)

# output module name
l_module := $(USRDEF_LOCAL_PROJECT_NAME)

# source files
l_src_file := \
	../lua521/lapi.c \
	../lua521/lauxlib.c \
	../lua521/lbaselib.c \
	../lua521/lbitlib.c \
	../lua521/lcode.c \
	../lua521/lcorolib.c \
	../lua521/lctype.c \
	../lua521/ldblib.c \
	../lua521/ldebug.c \
	../lua521/ldo.c \
	../lua521/ldump.c \
	../lua521/lfunc.c \
	../lua521/lgc.c \
	../lua521/linit.c \
	../lua521/liolib.c \
	../lua521/llex.c \
	../lua521/lmathlib.c \
	../lua521/lmem.c \
	../lua521/loadlib.c \
	../lua521/lobject.c \
	../lua521/lopcodes.c \
	../lua521/loslib.c \
	../lua521/lparser.c \
	../lua521/lstate.c \
	../lua521/lstring.c \
	../lua521/lstrlib.c \
	../lua521/ltable.c \
	../lua521/ltablib.c \
	../lua521/ltm.c \
	../lua521/lundump.c \
	../lua521/lvm.c \
	../lua521/lzio.c \
	luac.c \

# include path
l_c_include := \
	$(l_path) \
	$(l_path)/../include \
	$(USRDEF_APP_C_INCLUDE) \

# cflags
l_cflags := \
	-DANDROID \
	-DLUA_COMPAT_ALL \
	-DLUA_USE_LINUX \
	-D__ANDROID__ \
	-Wall \
	-Werror=endif-labels \
	-Werror=return-type \
	-Wno-comment \
	-Wno-enum-compare \
	-Wno-multichar \
	-Wno-sign-compare \
	-Wno-unused-local-typedefs \
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
		-Wl,--gc-sections \
		$(USRDEF_APP_LDFLAGS_RELEASE) \

endif

l_ldflags += \
	-lm \
	$(USRDEF_APP_LDFLAGS)\

# out_dir

ifeq ($(NDK_DEBUG), 1)
	l_app_dst := $(USRDEF_PROJ_OUTDIR)/bin-debug/$(TARGET_ARCH_ABI)
else
	l_app_dst := $(USRDEF_PROJ_OUTDIR)/bin/$(TARGET_ARCH_ABI)
endif

# build it

include $(CLEAR_VARS)

LOCAL_MODULE := $(l_module)
LOCAL_SRC_FILES := $(l_src_file)
LOCAL_C_INCLUDES := $(l_c_include)
LOCAL_CFLAGS := $(l_cflags)
LOCAL_CPPFLAGS := $(l_cppflags)
LOCAL_LDFLAGS := $(l_ldflags)

NDK_APP_DST_DIR := $(l_app_dst)

include $(BUILD_EXECUTABLE)
