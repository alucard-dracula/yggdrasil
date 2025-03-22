#
#	yggr_test_server_client definitions.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# subdirs
subdirs := \
	$(LOCAL_PATH)/../active_svr_test/Android_active_svr_test.mk \
	$(LOCAL_PATH)/../clt_test/Android_clt_test_async.mk \
	$(LOCAL_PATH)/../clt_test/Android_clt_test_static_task_center_balance2.mk \
	$(LOCAL_PATH)/../clt_test/Android_clt_test_static_task_center_balance.mk \
	$(LOCAL_PATH)/../clt_test/Android_clt_test_uuid_static_task_center_balance.mk \
	$(LOCAL_PATH)/../encryption_tool_clt_test/Android_encryption_tool_clt_test.mk \
	$(LOCAL_PATH)/../encryption_tool_proxy_srv_test/Android_encryption_tool_proxy_srv_test.mk \
	$(LOCAL_PATH)/../encryption_tool_proxy_trun_srv_test/Android_encryption_tool_proxy_trun_srv_test.mk \
	$(LOCAL_PATH)/../encryption_tool_svr_test/Android_encryption_tool_svr_test.mk \
	$(LOCAL_PATH)/../lua_active_svr_test/Android_lua_active_svr_test.mk \
	$(LOCAL_PATH)/../lua_clt_test/Android_lua_clt_test.mk \
	$(LOCAL_PATH)/../mixer_svr_test/Android_mixer_svr_test.mk \
	$(LOCAL_PATH)/../p2p_boot_svr_test/Android_p2p_boot_svr_test.mk \
	$(LOCAL_PATH)/../p2p_peer_test/Android_p2p_peer_test.mk \
	$(LOCAL_PATH)/../proxy_srv_test/Android_proxy_srv_test.mk \
	$(LOCAL_PATH)/../proxy_trun_mixer_svr_test/Android_proxy_trun_mixer_svr_test.mk \
	$(LOCAL_PATH)/../proxy_trun_srv_test/Android_proxy_trun_srv_test.mk \
	$(LOCAL_PATH)/../seh_svr_test/Android_seh_svr_test.mk \
	$(LOCAL_PATH)/../svr_test/Android_svr_test_async.mk \
	$(LOCAL_PATH)/../svr_test/Android_svr_test_balance_async.mk \
	$(LOCAL_PATH)/../svr_test/Android_svr_test_service_handler_shared_wrap_test.mk \
	$(LOCAL_PATH)/../svr_test/Android_svr_test_static_task_center_balance_async.mk \
	$(LOCAL_PATH)/../svr_test/Android_svr_test_uuid_static_task_center_balance_async.mk \
	$(LOCAL_PATH)/../udp_clt_test/Android_udp_clt_test.mk \
	$(LOCAL_PATH)/../udp_proxy_svr_test/Android_udp_proxy_svr_test.mk \
	$(LOCAL_PATH)/../udp_proxy_trun_svr_test/Android_udp_proxy_trun_svr_test.mk \
	$(LOCAL_PATH)/../udp_svr_test/Android_udp_svr_test.mk \


include $(subdirs)
