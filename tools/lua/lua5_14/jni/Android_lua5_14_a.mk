#
#	lua5_14 definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(LOCAL_PATH)/../lua514_static/Android_lua514_static.mk \
	$(LOCAL_PATH)/../lua514_ts_static/Android_lua514_ts_static.mk \


include $(subdirs)
