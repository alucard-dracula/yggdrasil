#
#	lzma definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(addprefix $(LOCAL_PATH)/../, $(addsuffix /Android.mk, \
		. \
	))

include $(subdirs)

