#
#	yggr_lib_only definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(LOCAL_PATH)/../yggr_lua_base_type/Android_yggr_lua_base_type.mk \


include $(subdirs)
