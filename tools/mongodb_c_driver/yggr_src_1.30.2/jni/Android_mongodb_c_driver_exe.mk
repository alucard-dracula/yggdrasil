#
#	mongodb_c_driver definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(LOCAL_PATH)/../mongoc3_test/Android_mongoc3_test.mk \


include $(subdirs)
