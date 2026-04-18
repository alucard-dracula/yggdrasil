#
#	lua5_34 definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(LOCAL_PATH)/../lua534_static/Android_lua534_static.mk \
	$(LOCAL_PATH)/../lua534_ts_static/Android_lua534_ts_static.mk \


include $(subdirs)
