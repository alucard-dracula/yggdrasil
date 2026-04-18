#
#	lua5_21 definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(LOCAL_PATH)/../lua521/Android_lua521.mk \
	$(LOCAL_PATH)/../lua521_ts/Android_lua521_ts.mk \


include $(subdirs)
