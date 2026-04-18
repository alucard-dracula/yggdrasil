#
#	lua5_14 definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(LOCAL_PATH)/../lua514/Android_lua514.mk \
	$(LOCAL_PATH)/../lua514_ts/Android_lua514_ts.mk \


include $(subdirs)
