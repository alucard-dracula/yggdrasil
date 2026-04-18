#
#	lua5_34 definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(LOCAL_PATH)/../lua534/Android_lua534.mk \
	$(LOCAL_PATH)/../lua534_ts/Android_lua534_ts.mk \


include $(subdirs)
