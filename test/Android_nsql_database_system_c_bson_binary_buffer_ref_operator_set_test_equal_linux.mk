#
#	nsql_database_system_c_bson_binary_buffer_ref_operator_set_test_equal definitions.
#

LOCAL_PATH := $(call my-dir)

USRDEF_LOCAL_PROJECT_NAME := nsql_database_system_c_bson_binary_buffer_ref_operator_set_test_equal

l_path := $(LOCAL_PATH)

# output module name
l_module := $(USRDEF_LOCAL_PROJECT_NAME)

# source files
l_src_file := \
	nsql_database_system/c_bson_binary_buffer_ref_operator_set_test_equal.cpp \

# include path
l_c_include := \
	$(l_path) \
	$(l_path)/../ \
	$(l_path)/../yggr_lua_base_type \
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
	-L$(USRDEF_PROJ_OUTDIR)/lib/$(TARGET_ARCH_ABI) \

ifeq ($(NDK_DEBUG), 1)
	l_ldflags += \
		-lyggr_nsql_database_system$(USRDEF_APP_COMPILER_VERSION_TAG)-d \
		-lyggr_charset$(USRDEF_APP_COMPILER_VERSION_TAG)-d \
		-lyggr_base$(USRDEF_APP_COMPILER_VERSION_TAG)-d \
		-lyggr_compatibility$(USRDEF_APP_COMPILER_VERSION_TAG)-d \
		-lboost_serialization$(USRDEF_APP_COMPILER_VERSION_TAG)-mt-sd$(tag_arch_bits)$(USRDEF_APP_BOOST_VERSION_TAG) \
		-lboost_thread$(USRDEF_APP_COMPILER_VERSION_TAG)-mt-d$(tag_arch_bits)$(USRDEF_APP_BOOST_VERSION_TAG) \
		-lmongoc$(USRDEF_APP_COMPILER_VERSION_TAG)-d \
		-lmongocrypt$(USRDEF_APP_COMPILER_VERSION_TAG)-d \
		-lkms-message$(USRDEF_APP_COMPILER_VERSION_TAG)-d \
		-lbson$(USRDEF_APP_COMPILER_VERSION_TAG)-d \
		-lcommon$(USRDEF_APP_COMPILER_VERSION_TAG)-d \
		-lbson$(USRDEF_APP_COMPILER_VERSION_TAG)-d \
		-lcommon$(USRDEF_APP_COMPILER_VERSION_TAG)-d \
		-lutf8proc$(USRDEF_APP_COMPILER_VERSION_TAG)-d \
		-licuuc \
		-licui18n \
		-licudata \
		-lbase64$(USRDEF_APP_COMPILER_VERSION_TAG)-d \
		$(USRDEF_APP_LDFLAGS_DEBUG) \

else
	l_ldflags += \
		-s \
		-Wl,--gc-sections \
		-lyggr_nsql_database_system$(USRDEF_APP_COMPILER_VERSION_TAG) \
		-lyggr_charset$(USRDEF_APP_COMPILER_VERSION_TAG) \
		-lyggr_base$(USRDEF_APP_COMPILER_VERSION_TAG) \
		-lyggr_compatibility$(USRDEF_APP_COMPILER_VERSION_TAG) \
		-lboost_serialization$(USRDEF_APP_COMPILER_VERSION_TAG)-mt-s$(tag_arch_bits)$(USRDEF_APP_BOOST_VERSION_TAG) \
		-lboost_thread$(USRDEF_APP_COMPILER_VERSION_TAG)-mt$(tag_arch_bits)$(USRDEF_APP_BOOST_VERSION_TAG) \
		-lmongoc$(USRDEF_APP_COMPILER_VERSION_TAG) \
		-lmongocrypt$(USRDEF_APP_COMPILER_VERSION_TAG) \
		-lkms-message$(USRDEF_APP_COMPILER_VERSION_TAG) \
		-lbson$(USRDEF_APP_COMPILER_VERSION_TAG) \
		-lcommon$(USRDEF_APP_COMPILER_VERSION_TAG) \
		-lbson$(USRDEF_APP_COMPILER_VERSION_TAG) \
		-lcommon$(USRDEF_APP_COMPILER_VERSION_TAG) \
		-lutf8proc$(USRDEF_APP_COMPILER_VERSION_TAG) \
		-licuuc \
		-licui18n \
		-licudata \
		-lbase64$(USRDEF_APP_COMPILER_VERSION_TAG) \
		$(USRDEF_APP_LDFLAGS_RELEASE) \

endif

l_ldflags += \
	-lz \
	-lssl \
	-liconv \
	-lcrypto \
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
