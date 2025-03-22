#
#	lua5_21 definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(LOCAL_PATH)/../lua521_static/Android_lua521_static.mk \
	$(LOCAL_PATH)/../lua521_static_ts/Android_lua521_static_ts.mk \


include $(subdirs)
