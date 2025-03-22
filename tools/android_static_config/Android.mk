LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := yggr_base
LOCAL_SRC_FILES := $(NDK_ROOT)/sources/yggdrasil/lib/libyggr_base.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := yggr_charset
LOCAL_SRC_FILES := $(NDK_ROOT)/sources/yggdrasil/lib/libyggr_charset.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := yggr_ids
LOCAL_SRC_FILES := $(NDK_ROOT)/sources/yggdrasil/lib/libyggr_ids.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := yggr_time
LOCAL_SRC_FILES := $(NDK_ROOT)/sources/yggdrasil/lib/libyggr_time.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := compatibility
LOCAL_SRC_FILES := $(NDK_ROOT)/sources/yggdrasil/lib/libcompatibility.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)

