#
#	yggr_any_val definitions.
#

LOCAL_PATH := $(call my-dir)

USRDEF_LOCAL_PROJECT_NAME := yggr_any_val

l_path := $(LOCAL_PATH)

# output module name
ifeq ($(NDK_DEBUG), 1)
	l_module := lib$(USRDEF_LOCAL_PROJECT_NAME)$(USRDEF_APP_COMPILER_VERSION_TAG)-d
else
	l_module := lib$(USRDEF_LOCAL_PROJECT_NAME)$(USRDEF_APP_COMPILER_VERSION_TAG)
endif

# source files
l_src_file := \
	any_operator_mgr_basic_t_reg.cpp \
	any_operator_mgr_basic_t_reg_bitwise_op_and_eins.cpp \
	any_operator_mgr_basic_t_reg_bitwise_op_and_zwei.cpp \
	any_operator_mgr_basic_t_reg_bitwise_op_left_shift_eins.cpp \
	any_operator_mgr_basic_t_reg_bitwise_op_left_shift_zwei.cpp \
	any_operator_mgr_basic_t_reg_bitwise_op_not.cpp \
	any_operator_mgr_basic_t_reg_bitwise_op_or_eins.cpp \
	any_operator_mgr_basic_t_reg_bitwise_op_or_zwei.cpp \
	any_operator_mgr_basic_t_reg_bitwise_op_right_shift_enis.cpp \
	any_operator_mgr_basic_t_reg_bitwise_op_right_shift_zwei.cpp \
	any_operator_mgr_basic_t_reg_bitwise_op_xor_eins.cpp \
	any_operator_mgr_basic_t_reg_bitwise_op_xor_zwei.cpp \
	any_operator_mgr_basic_t_reg_logic_op_and.cpp \
	any_operator_mgr_basic_t_reg_logic_op_equal_to_eins.cpp \
	any_operator_mgr_basic_t_reg_logic_op_equal_to_zwei.cpp \
	any_operator_mgr_basic_t_reg_logic_op_greater_eins.cpp \
	any_operator_mgr_basic_t_reg_logic_op_greater_equal_eins.cpp \
	any_operator_mgr_basic_t_reg_logic_op_greater_equal_zwei.cpp \
	any_operator_mgr_basic_t_reg_logic_op_greater_zwei.cpp \
	any_operator_mgr_basic_t_reg_logic_op_less_eins.cpp \
	any_operator_mgr_basic_t_reg_logic_op_less_equal_eins.cpp \
	any_operator_mgr_basic_t_reg_logic_op_less_equal_zwei.cpp \
	any_operator_mgr_basic_t_reg_logic_op_less_zwei.cpp \
	any_operator_mgr_basic_t_reg_logic_op_not.cpp \
	any_operator_mgr_basic_t_reg_logic_op_not_equal_to_eins.cpp \
	any_operator_mgr_basic_t_reg_logic_op_not_equal_to_zwei.cpp \
	any_operator_mgr_basic_t_reg_logic_op_or.cpp \
	any_operator_mgr_basic_t_reg_math_bin_op_div_eins.cpp \
	any_operator_mgr_basic_t_reg_math_bin_op_div_zwei.cpp \
	any_operator_mgr_basic_t_reg_math_bin_op_minus_eins.cpp \
	any_operator_mgr_basic_t_reg_math_bin_op_minus_zwei.cpp \
	any_operator_mgr_basic_t_reg_math_bin_op_mod_eins.cpp \
	any_operator_mgr_basic_t_reg_math_bin_op_mod_zwei.cpp \
	any_operator_mgr_basic_t_reg_math_bin_op_mul_eins.cpp \
	any_operator_mgr_basic_t_reg_math_bin_op_mul_zwei.cpp \
	any_operator_mgr_basic_t_reg_math_bin_op_plus_drei.cpp \
	any_operator_mgr_basic_t_reg_math_bin_op_plus_eins.cpp \
	any_operator_mgr_basic_t_reg_math_bin_op_plus_funf.cpp \
	any_operator_mgr_basic_t_reg_math_bin_op_plus_zwei.cpp \
	any_operator_mgr_basic_t_reg_math_bin_op_set_eins.cpp \
	any_operator_mgr_basic_t_reg_math_bin_op_set_zwei.cpp \
	any_operator_mgr_basic_t_reg_math_unary_op_eins.cpp \
	any_operator_mgr_basic_t_reg_math_unary_op_zwei.cpp \
	any_operator_mgr_boost_basic_t_reg.cpp \
	any_operator_mgr_boost_basic_t_reg_bitwise_op_and_eins.cpp \
	any_operator_mgr_boost_basic_t_reg_bitwise_op_and_zwei.cpp \
	any_operator_mgr_boost_basic_t_reg_bitwise_op_left_shift_eins.cpp \
	any_operator_mgr_boost_basic_t_reg_bitwise_op_left_shift_zwei.cpp \
	any_operator_mgr_boost_basic_t_reg_bitwise_op_not.cpp \
	any_operator_mgr_boost_basic_t_reg_bitwise_op_or_eins.cpp \
	any_operator_mgr_boost_basic_t_reg_bitwise_op_or_zwei.cpp \
	any_operator_mgr_boost_basic_t_reg_bitwise_op_right_shift_enis.cpp \
	any_operator_mgr_boost_basic_t_reg_bitwise_op_right_shift_zwei.cpp \
	any_operator_mgr_boost_basic_t_reg_bitwise_op_xor_eins.cpp \
	any_operator_mgr_boost_basic_t_reg_bitwise_op_xor_zwei.cpp \
	any_operator_mgr_boost_basic_t_reg_logic_op_and.cpp \
	any_operator_mgr_boost_basic_t_reg_logic_op_equal_to_eins.cpp \
	any_operator_mgr_boost_basic_t_reg_logic_op_equal_to_zwei.cpp \
	any_operator_mgr_boost_basic_t_reg_logic_op_greater_eins.cpp \
	any_operator_mgr_boost_basic_t_reg_logic_op_greater_equal_eins.cpp \
	any_operator_mgr_boost_basic_t_reg_logic_op_greater_equal_zwei.cpp \
	any_operator_mgr_boost_basic_t_reg_logic_op_greater_zwei.cpp \
	any_operator_mgr_boost_basic_t_reg_logic_op_less_eins.cpp \
	any_operator_mgr_boost_basic_t_reg_logic_op_less_equal_eins.cpp \
	any_operator_mgr_boost_basic_t_reg_logic_op_less_equal_zwei.cpp \
	any_operator_mgr_boost_basic_t_reg_logic_op_less_zwei.cpp \
	any_operator_mgr_boost_basic_t_reg_logic_op_not.cpp \
	any_operator_mgr_boost_basic_t_reg_logic_op_not_equal_to_eins.cpp \
	any_operator_mgr_boost_basic_t_reg_logic_op_not_equal_to_zwei.cpp \
	any_operator_mgr_boost_basic_t_reg_logic_op_or.cpp \
	any_operator_mgr_boost_basic_t_reg_math_bin_op_div_eins.cpp \
	any_operator_mgr_boost_basic_t_reg_math_bin_op_div_zwei.cpp \
	any_operator_mgr_boost_basic_t_reg_math_bin_op_minus_eins.cpp \
	any_operator_mgr_boost_basic_t_reg_math_bin_op_minus_zwei.cpp \
	any_operator_mgr_boost_basic_t_reg_math_bin_op_mod_eins.cpp \
	any_operator_mgr_boost_basic_t_reg_math_bin_op_mod_zwei.cpp \
	any_operator_mgr_boost_basic_t_reg_math_bin_op_mul_eins.cpp \
	any_operator_mgr_boost_basic_t_reg_math_bin_op_mul_zwei.cpp \
	any_operator_mgr_boost_basic_t_reg_math_bin_op_plus_drei.cpp \
	any_operator_mgr_boost_basic_t_reg_math_bin_op_plus_eins.cpp \
	any_operator_mgr_boost_basic_t_reg_math_bin_op_plus_funf.cpp \
	any_operator_mgr_boost_basic_t_reg_math_bin_op_plus_zwei.cpp \
	any_operator_mgr_boost_basic_t_reg_math_bin_op_set_eins.cpp \
	any_operator_mgr_boost_basic_t_reg_math_bin_op_set_zwei.cpp \
	any_operator_mgr_boost_basic_t_reg_math_unary_op_eins.cpp \
	any_operator_mgr_boost_basic_t_reg_math_unary_op_zwei.cpp \

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
		-Og \
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
