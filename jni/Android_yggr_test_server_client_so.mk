#
#	yggr_test_server_client definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(LOCAL_PATH)/../dll_cal_test/Android_dll_cal_test.mk \
	$(LOCAL_PATH)/../lua_cal_svr_procotol/Android_lua_cal_svr_procotol.mk \
	$(LOCAL_PATH)/../lua_calculator/Android_lua_calculator.mk \
	$(LOCAL_PATH)/../lua_hello_dll/Android_lua_hello_dll.mk \
	$(LOCAL_PATH)/../yggr_lua_base_type/Android_yggr_lua_base_type.mk \


include $(subdirs)
