#
#	lua5_21 definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(LOCAL_PATH)/../lua/Android_lua.mk \
	$(LOCAL_PATH)/../luac/Android_luac.mk \


include $(subdirs)
