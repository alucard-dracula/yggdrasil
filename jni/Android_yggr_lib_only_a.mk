#
#	yggr_lib_only definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(LOCAL_PATH)/../yggr_any_val/Android_yggr_any_val.mk \
	$(LOCAL_PATH)/../yggr_base/Android_yggr_base.mk \
	$(LOCAL_PATH)/../yggr_charset/Android_yggr_charset.mk \
	$(LOCAL_PATH)/../yggr_compaction_tool/Android_yggr_compaction_tool.mk \
	$(LOCAL_PATH)/../yggr_compatibility/Android_yggr_compatibility.mk \
	$(LOCAL_PATH)/../yggr_database_system/Android_yggr_database_system.mk \
	$(LOCAL_PATH)/../yggr_encryption_tool/Android_yggr_encryption_tool.mk \
	$(LOCAL_PATH)/../yggr_exception/Android_yggr_exception.mk \
	$(LOCAL_PATH)/../yggr_ids/Android_yggr_ids.mk \
	$(LOCAL_PATH)/../yggr_log/Android_yggr_log.mk \
	$(LOCAL_PATH)/../yggr_modules/Android_yggr_modules.mk \
	$(LOCAL_PATH)/../yggr_script_lua/Android_yggr_script_lua.mk \
	$(LOCAL_PATH)/../yggr_script_python/Android_yggr_script_python.mk \
	$(LOCAL_PATH)/../yggr_segment/Android_yggr_segment.mk \
	$(LOCAL_PATH)/../yggr_seh/Android_yggr_seh.mk \
	$(LOCAL_PATH)/../yggr_system_controller/Android_yggr_system_controller.mk \
	$(LOCAL_PATH)/../yggr_time/Android_yggr_time.mk \


include $(subdirs)
