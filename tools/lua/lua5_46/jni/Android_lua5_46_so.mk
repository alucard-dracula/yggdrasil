#
#	lua5_46 definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(LOCAL_PATH)/../lua546/Android_lua546.mk \
	$(LOCAL_PATH)/../lua546_ts/Android_lua546_ts.mk \


include $(subdirs)
