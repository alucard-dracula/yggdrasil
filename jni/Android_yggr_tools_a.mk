#
#	yggr_tools definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(LOCAL_PATH)/../yggr_base/Android_yggr_base.mk \
	$(LOCAL_PATH)/../yggr_charset/Android_yggr_charset.mk \
	$(LOCAL_PATH)/../yggr_compatibility/Android_yggr_compatibility.mk \
	$(LOCAL_PATH)/../yggr_encryption_tool/Android_yggr_encryption_tool.mk \
	$(LOCAL_PATH)/../yggr_exception/Android_yggr_exception.mk \
	$(LOCAL_PATH)/../yggr_ids/Android_yggr_ids.mk \
	$(LOCAL_PATH)/../yggr_system_controller/Android_yggr_system_controller.mk \
	$(LOCAL_PATH)/../yggr_time/Android_yggr_time.mk \


include $(subdirs)
