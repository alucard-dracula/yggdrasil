#
#	lua5_46 definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(LOCAL_PATH)/../lua546_static/Android_lua546_static.mk \
	$(LOCAL_PATH)/../lua546_ts_static/Android_lua546_ts_static.mk \


include $(subdirs)
