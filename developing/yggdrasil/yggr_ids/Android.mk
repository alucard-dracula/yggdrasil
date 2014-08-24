
LOCAL_PATH:= $(call my-dir)

#
# yggr_ids definitions.
#

src_files := inner_process_id.cpp uuid.cpp

c_includes := \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/../compatibility \
	$(LOCAL_PATH)/.. \
	$(NDK_ROOT)/sources/boost_1_53_0 \


#local_cflags += -DYGGR_SERIALIZE_SIZE32
#local_cflags += -DLINUX
#local_cflags += -DYGGR_SYS_DEFAULT_STRING_CHARSET="utf8"

local_cflags += -O3 -fvisibility=hidden

include $(CLEAR_VARS)
#LOCAL_C_INCLUDES := $(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.8/include
LOCAL_SDK_VERSION := 8
LOCAL_NDK_STL_VARIANT := gnustl_static
LOCAL_C_INCLUDES += $(c_includes)
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)
LOCAL_CPP_FEATURES := rtti
LOCAL_CFLAGS += $(local_cflags) -DPIC -fPIC -frtti
LOCAL_CFLAGS += -D__GLIBC__ -lm -lc
# Using -Os over -O3 actually cuts down the final executable size by a few dozen kilobytes
LOCAL_CFLAGS += -O3
#LOCAL_EXPORT_CFLAGS += -DU_STATIC_IMPLEMENTATION=1
LOCAL_CFLAGS += -DU_STATIC_IMPLEMENTATION=1 -D__ANDROID__ -DANDROID -fdata-sections -ffunction-sections
LOCAL_CPPFLAGS += -DU_STATIC_IMPLEMENTATION=1 -D__ANDROID__ -DANDROID -frtti -fexceptions -fdata-sections -ffunction-sections
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libyggr_ids
LOCAL_SRC_FILES += $(src_files)
include $(BUILD_STATIC_LIBRARY)
