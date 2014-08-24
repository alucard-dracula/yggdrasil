
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)


subdirs := $(addprefix $(LOCAL_PATH)/,$(addsuffix /Android.mk, \
		yggdrasil/lib_compatibility \
		yggdrasil/yggr_time \
		yggdrasil/yggr_ids \
		yggdrasil/yggr_base \
		yggdrasil/yggr_charset \
		yggdrasil/yggr_modules \
	))

include $(subdirs)
