// tool_conv_to_darwin_cmakelists_txt_cfg.cpp

#include "tool_conv_to_darwin_cmakelists_txt_cfg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#if !(BOOST_VERSION < 105600)

tool_conv_to_darwin_cmakelists_txt_cfg::tool_conv_to_darwin_cmakelists_txt_cfg(void)
{
}

tool_conv_to_darwin_cmakelists_txt_cfg::tool_conv_to_darwin_cmakelists_txt_cfg(tool_conv_to_darwin_cmakelists_txt_cfg_sample)
	: root_dir_("..")
{
	dir_filter_.insert("test_data");
	dir_filter_.insert("lib");
	dir_filter_.insert("lib64");
	dir_filter_.insert("test_out");
	dir_filter_.insert("tools");
	dir_filter_.insert("Debug");
	dir_filter_.insert("Debug64");
	dir_filter_.insert("Release");
	dir_filter_.insert("Release64");
	dir_filter_.insert("Debug-Win32");
	dir_filter_.insert("Debug-x64");
	dir_filter_.insert("Release-Win32");
	dir_filter_.insert("Release-x64");
	dir_filter_.insert("jni");
	dir_filter_.insert(".git");
	dir_filter_.insert(".vs");
	dir_filter_.insert(".vscode");
	dir_filter_.insert("discard");
	dir_filter_.insert("licenses");
	dir_filter_.insert("need_reconstruction");
	dir_filter_.insert("yggdrasil-ios.xcworkspace");
	dir_filter_.insert("active_svr_test");
	dir_filter_.insert("android_config");
	dir_filter_.insert("clt_test");
	dir_filter_.insert("dll_cal_test");
	dir_filter_.insert("dll_param_test");
	dir_filter_.insert("encryption_tool_clt_test");
	dir_filter_.insert("encryption_tool_proxy_srv_test");
	dir_filter_.insert("encryption_tool_proxy_trun_srv_test");
	dir_filter_.insert("encryption_tool_svr_test");
	dir_filter_.insert("licenses");
	dir_filter_.insert("lua_active_svr_test");
	dir_filter_.insert("lua_cal_svr_procotol");
	dir_filter_.insert("lua_calculator");
	dir_filter_.insert("lua_clt_test");
	dir_filter_.insert("lua_hello_dll");
	dir_filter_.insert("mixer_svr_test");
	dir_filter_.insert("p2p_boot_svr_test");
	dir_filter_.insert("p2p_peer_test");
	dir_filter_.insert("proxy_srv_test");
	dir_filter_.insert("proxy_trun_mixer_svr_test");
	dir_filter_.insert("proxy_trun_srv_test");
	dir_filter_.insert("pyd_export");
	dir_filter_.insert("seh_svr_test");
	dir_filter_.insert("svr_test");
	dir_filter_.insert("test");
	dir_filter_.insert("tool_conv_to_cb_solution_linux");
	dir_filter_.insert("tool_conv_to_cb_solution_win_mingw");
	dir_filter_.insert("tool_conv_to_jni_mk");
	dir_filter_.insert("tool_downgrade_vcxproj");
	dir_filter_.insert("tool_format_vcxproj");
	dir_filter_.insert("tool_remove_duplicate_files");
	dir_filter_.insert("tool_suo_batch_build_selector");
	dir_filter_.insert("tool_test_project_maker");
	dir_filter_.insert("tool_test_search_files");
	dir_filter_.insert("tool_upgrade_vcxproj");
	dir_filter_.insert("udp_clt_test");
	dir_filter_.insert("udp_proxy_svr_test");
	dir_filter_.insert("udp_proxy_trun_svr_test");
	dir_filter_.insert("udp_svr_test");
	dir_filter_.insert("yggr");
	dir_filter_.insert("yggr_any_val");
	dir_filter_.insert("yggr_base");
	dir_filter_.insert("yggr_charset");
	dir_filter_.insert("yggr_compaction_tool");
	dir_filter_.insert("yggr_compatibility");
	dir_filter_.insert("yggr_database_system");
	dir_filter_.insert("yggr_encryption_tool");
	dir_filter_.insert("yggr_exception");
	dir_filter_.insert("yggr_ids");
	dir_filter_.insert("yggr_log");
	dir_filter_.insert("yggr_lua_base_type");
	dir_filter_.insert("yggr_modules");
	dir_filter_.insert("yggr_nsql_database_system");
	dir_filter_.insert("yggr_script_lua");
	dir_filter_.insert("yggr_script_python");
	dir_filter_.insert("yggr_segment");
	dir_filter_.insert("yggr_seh");
	dir_filter_.insert("yggr_system_controller");
	dir_filter_.insert("yggr_time");

	cbp_file_filter_.insert("include_linux.cbp");
	cbp_file_filter_.insert("yggr_linux.cbp");
	cbp_file_filter_.insert("dll_param_test_linux.cbp");
	//cbp_file_filter_.insert("yggr_nsql_database_system_linux.cbp");
	//cbp_file_filter_.insert("pyd_export_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_base_bson_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_bson_serialize_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_basic_demo_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_basic_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_binary_buffer_ref_operator_set_test_equal_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_binary_buffer_ref_operator_set_test_liner_only_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_binary_buffer_ref_operator_set_test_sizeof_greater_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_binary_buffer_ref_operator_set_test_sizeof_less_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_binary_buffer_ref_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_binary_buffer_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_code_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_code_w_scope_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_date_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_date_time_timeval_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_dbpointer_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_decimal128_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_error_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_oid_test2_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_oid_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_regex_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_symbol_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_time_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_timestamp_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_timeval_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_value_test2_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_bson_value_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_apm_callbacks_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_auto_encryption_opts_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_client_encryption_datakey_opts_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_client_encryption_encrypt_opts_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_client_encryption_encrypt_range_opts_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_client_encryption_opts_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_client_encryption_rewrap_many_datakey_result_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_connection_init_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_find_and_modify_opts_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_gridfs_file_opt_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_gridfs_file_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_host_info_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_index_model_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_read_concern_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_read_prefs_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_server_api_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_ssl_opt_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_stream_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_uri_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_c_mongo_write_concern_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_cert_file_win32_load_failed_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_mongo_accesser_encryption_test_of_connect_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_mongo_accesser_encryption_test_of_connect_pool_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_mongo_accesser_test_of_connection_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_mongo_accesser_test_of_connection_mt_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_mongo_accesser_test_of_connection_pool_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_mongo_accesser_test_of_connection_pool_mt_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_mongo_accesser_test_of_connection_sharding_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_mongo_gridfs_bucket_org_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_mongo_gridfs_org_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_mongo_operators_dollar_cmd_def_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_mongoc_connection_pool_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_mongodbc_basic_connect_test_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_test_mongodb_async_mgr_linux.cbp");
	//cbp_file_filter_.insert("nsql_database_system_test_mongodb_mgr_linux.cbp");

	cbp_file_filter_.insert("test_linux.cbp");
	cbp_file_filter_.insert("test_other_linux.cbp");
	cbp_file_filter_.insert("test_other2_linux.cbp");
	cbp_file_filter_.insert("test_other3_linux.cbp");

	//cbp_file_filter_.insert("any_val_any_bson_serialize_test_linux.cbp");
	//cbp_file_filter_.insert("any_val_params_bson_serialize_test_linux.cbp");
	//cbp_file_filter_.insert("charset_utf8_string_bson_pak_test_linux.cbp");
	//cbp_file_filter_.insert("exception_bson_pak_test_of_error_code_linux.cbp");
	//cbp_file_filter_.insert("ids_uuid_bson_serialize_test_linux.cbp");

	//cbp_file_filter_.insert("packet_bson_cmd_pak_test_of_container_midx_linux.cbp");
	//cbp_file_filter_.insert("packet_bson_cmd_pak_test_of_container_part1_linux.cbp");
	//cbp_file_filter_.insert("packet_bson_cmd_pak_test_of_container_part2_linux.cbp");
	//cbp_file_filter_.insert("packet_bson_pak_test_of_class_and_base_linux.cbp");
	//cbp_file_filter_.insert("packet_bson_pak_test_of_container_part1_1_linux.cbp");
	//cbp_file_filter_.insert("packet_bson_pak_test_of_container_part1_2_linux.cbp");
	//cbp_file_filter_.insert("packet_bson_pak_test_of_container_part2_1_linux.cbp");
	//cbp_file_filter_.insert("packet_bson_pak_test_of_container_part2_2_linux.cbp");
	//cbp_file_filter_.insert("packet_bson_pak_test_of_container_part3_linux.cbp");
	//cbp_file_filter_.insert("packet_bson_pak_test_of_container_part4_linux.cbp");
	//cbp_file_filter_.insert("packet_bson_pak_test_of_tuple_container_linux.cbp");
	//cbp_file_filter_.insert("packet_bug_test_bson_pak_man_linux.cbp");
	//cbp_file_filter_.insert("packet_shared_ptr_bson_serialization_test_linux.cbp");
	//cbp_file_filter_.insert("packet_test_bson_serialization_include_linux.cbp");

	//cbp_file_filter_.insert("compiler_link_test_msvc_maroc_linux.cbp");

	//cbp_file_filter_.insert("exception_exception_mongodb_log_test_linux.cbp");
	//cbp_file_filter_.insert("log_log_mongodb_accesser_test_linux.cbp");
	//cbp_file_filter_.insert("log_log_mongodb_op_caller_test_linux.cbp");
	//cbp_file_filter_.insert("log_log_mongodb_op_test_linux.cbp");

	//cpp_file_filter_.insert("log_mongodb_op_caller.cpp");
}

tool_conv_to_darwin_cmakelists_txt_cfg::tool_conv_to_darwin_cmakelists_txt_cfg(const this_type& right)
	: root_dir_(right.root_dir_),
		dir_filter_(right.dir_filter_),
		cbp_file_filter_(right.cbp_file_filter_),
		cpp_file_filter_(right.cpp_file_filter_)
{
}

tool_conv_to_darwin_cmakelists_txt_cfg::~tool_conv_to_darwin_cmakelists_txt_cfg(void)
{
}

tool_conv_to_darwin_cmakelists_txt_cfg::this_type& 
	tool_conv_to_darwin_cmakelists_txt_cfg::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	root_dir_ = right.root_dir_;
	dir_filter_ = right.dir_filter_;
	cbp_file_filter_ = right.cbp_file_filter_;
	cpp_file_filter_ = right.cpp_file_filter_;

	return *this;
}

void tool_conv_to_darwin_cmakelists_txt_cfg::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	yggr::swap(root_dir_, right.root_dir_);
	yggr::swap(dir_filter_, right.dir_filter_);
	yggr::swap(cbp_file_filter_, right.cbp_file_filter_);
	yggr::swap(cpp_file_filter_, right.cpp_file_filter_);
}

namespace swap_support
{
	YGGR_PP_SWAP_THIS_REF_DEF_IMPL(tool_conv_to_darwin_cmakelists_txt_cfg)
} // namespace swap_support

#endif // #if !(BOOST_VERSION < 105600)