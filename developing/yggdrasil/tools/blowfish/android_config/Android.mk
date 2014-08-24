LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := blowfish
LOCAL_SRC_FILES := $(NDK_ROOT)/sources/libblowfish/lib/libblowfish.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)
