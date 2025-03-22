#
#	yggr_tools definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(LOCAL_PATH)/../tool_conv_to_cb_solution_linux/Android_tool_conv_to_cb_solution_linux.mk \
	$(LOCAL_PATH)/../tool_conv_to_cb_solution_win_mingw/Android_tool_conv_to_cb_solution_win_mingw.mk \
	$(LOCAL_PATH)/../tool_conv_to_darwin_cmakelists_txt/Android_tool_conv_to_darwin_cmakelists_txt.mk \
	$(LOCAL_PATH)/../tool_conv_to_jni_mk/Android_tool_conv_to_jni_mk.mk \
	$(LOCAL_PATH)/../tool_format_vcxproj/Android_tool_format_vcxproj.mk \
	$(LOCAL_PATH)/../tool_remove_duplicate_files/Android_tool_remove_duplicate_files.mk \
	$(LOCAL_PATH)/../tool_suo_batch_build_selector/Android_tool_suo_batch_build_selector.mk \
	$(LOCAL_PATH)/../tool_test_project_maker/Android_tool_test_project_maker.mk \
	$(LOCAL_PATH)/../tool_test_search_files/Android_tool_test_search_files.mk \
	$(LOCAL_PATH)/../tool_upgrade_vcxproj/Android_tool_upgrade_vcxproj.mk \


include $(subdirs)
