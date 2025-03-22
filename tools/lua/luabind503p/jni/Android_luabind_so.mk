#
#	luabind_linux_ definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(LOCAL_PATH)/../luabind/Android_luabind.mk \


ifneq ("$(TARGET_ARCH_ABI)", "riscv64")
	include $(subdirs)
else
	ifneq ("$(USRDEF_APP_LUA_VERSION)", "jit501")
		include $(subdirs)
	endif
endif