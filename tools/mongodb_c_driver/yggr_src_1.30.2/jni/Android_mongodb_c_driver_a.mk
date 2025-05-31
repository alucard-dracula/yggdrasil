#
#	mongodb_c_driver definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(LOCAL_PATH)/../bson/Android_bson.mk \
	$(LOCAL_PATH)/../common/Android_common.mk \
	$(LOCAL_PATH)/../kms-message/Android_kms-message.mk \
	$(LOCAL_PATH)/../mongoc/Android_mongoc.mk \
	$(LOCAL_PATH)/../mongocrypt/Android_mongocrypt.mk \
	$(LOCAL_PATH)/../utf8proc/Android_utf8proc.mk \


include $(subdirs)
