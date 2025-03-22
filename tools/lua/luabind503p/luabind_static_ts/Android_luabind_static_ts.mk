#
#	luabind_static_ts definitions.
#

LOCAL_PATH := $(call my-dir)

USRDEF_LOCAL_PROJECT_NAME := luabind_static_ts

l_path := $(LOCAL_PATH)

# output module name
ifeq ($(NDK_DEBUG), 1)
	l_module := libluabind09_ts$(USRDEF_APP_COMPILER_VERSION_TAG)-s-d
else
	l_module := libluabind09_ts$(USRDEF_APP_COMPILER_VERSION_TAG)-s
endif

# source files
l_src_file := \
	../luabind/class.cpp \
	../luabind/class_info.cpp \
	../luabind/class_registry.cpp \
	../luabind/class_rep.cpp \
	../luabind/create_class.cpp \
	../luabind/error.cpp \
	../luabind/exception_handler.cpp \
	../luabind/function.cpp \
	../luabind/inheritance.cpp \
	../luabind/link_compatibility.cpp \
	../luabind/object_rep.cpp \
	../luabind/open.cpp \
	../luabind/pcall.cpp \
	../luabind/scope.cpp \
	../luabind/stack_content_by_name.cpp \
	../luabind/weak_ref.cpp \
	../luabind/wrapper_base.cpp \

# include path
l_c_include := \
	$(l_path) \
	$(l_path)/../include \
	$(USRDEF_APP_C_INCLUDE) \

# cflags
l_cflags := \
	-DANDROID \
	-DYGGR_LUA_THREAD_SAFE \
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
		-O0 \
		-fno-elide-constructors \
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
