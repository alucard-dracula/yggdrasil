// lib_depend_info.cpp

#include "lib_depend_info.hpp"

#if !(BOOST_VERSION < 105600)

namespace lib
{

lib_depend_info::lib_depend_info(void)
{
}

lib_depend_info::lib_depend_info(lib_depend_info_cfg_sample)
{
	_str_boost_ver = "1_82";
	_str_lua_ver = "503";
	_str_python_ver = "312";

	// marks_depend
	{
		_marks_dep["any_val"].insert("yggr_any_val");
		//_marks_dep["any_val_boost"].insert("yggr_any_val_boost");

		_marks_dep["base"].insert("yggr_base");
		_marks_dep["base"].insert("yggr_compatibility");

		_marks_dep["charset"].insert("yggr_charset");
		_marks_dep["charset"].insert("iconv");
		_marks_dep["charset"].insert("ws2_32");

		_marks_dep["compaction_tool"].insert("yggr_compaction_tool");
		_marks_dep["compaction_tool"].insert("lzma");

		_marks_dep["database_system"].insert("yggr_database_system");
		_marks_dep["database_system"].insert("dtl");
		_marks_dep["database_system"].insert("odbc32");
		_marks_dep["database_system"].insert("ws2_32");

		_marks_dep["encryption_tool"].insert("yggr_encryption_tool");
		_marks_dep["encryption_tool"].insert("blowfish");
		_marks_dep["encryption_tool"].insert("sha");
		_marks_dep["encryption_tool"].insert("md5");
		_marks_dep["encryption_tool"].insert("ws2_32");

		_marks_dep["exception"].insert("yggr_exception");

		_marks_dep["ids"].insert("yggr_ids");

		_marks_dep["log"].insert("yggr_log");
		_marks_dep["log"].insert("boost_filesystem");
		_marks_dep["log"].insert("boost_regex");

		_marks_dep["modules"].insert("yggr_modules");

		_marks_dep["nsql_database_system"].insert("yggr_nsql_database_system");
		_marks_dep["nsql_database_system"].insert("boost_serialization");
		_marks_dep["nsql_database_system"].insert("boost_thread");
		_marks_dep["nsql_database_system"].insert("mongoc");
		_marks_dep["nsql_database_system"].insert("bson");
		_marks_dep["nsql_database_system"].insert("common");
		_marks_dep["nsql_database_system"].insert("mongocrypt");
		_marks_dep["nsql_database_system"].insert("kms-message");
		_marks_dep["nsql_database_system"].insert("utf8proc");
		_marks_dep["nsql_database_system"].insert("intel_dfp_obj");
		_marks_dep["nsql_database_system"].insert("zlibstatic");
		_marks_dep["nsql_database_system"].insert("base64");
		_marks_dep["nsql_database_system"].insert("icuuc");
		_marks_dep["nsql_database_system"].insert("icuin");
		_marks_dep["nsql_database_system"].insert("sasl");
		_marks_dep["nsql_database_system"].insert("ws2_32");
		_marks_dep["nsql_database_system"].insert("Dnsapi");
		//_marks_dep["nsql_database_system"].insert("Bcrypt");
		//_marks_dep["nsql_database_system"].insert("Secur32");
		//_marks_dep["nsql_database_system"].insert("Crypt32");
		_marks_dep["nsql_database_system"].insert("crypto");
		_marks_dep["nsql_database_system"].insert("ssl");

		_marks_dep["mongoc"].insert("mongoc");
		_marks_dep["bson"].insert("bson");
		_marks_dep["common"].insert("common");
		_marks_dep["kms-message"].insert("kms-message");
		_marks_dep["mongocrypt"].insert("mongocrypt");
		_marks_dep["mongocrypt"].insert("intel_dfp_obj");
		_marks_dep["mongocrypt"].insert("sasl");
		_marks_dep["utf8proc"].insert("utf8proc");

		// icu supported mingw
		_marks_dep["regular_parse"].insert("boost_regex");
		_marks_dep["regular_parse"].insert("icuuc");
		_marks_dep["regular_parse"].insert("icuin");

		// yggr_include_marks beg
		_marks_dep["yggr_network"].insert("boost_serialization");
		_marks_dep["yggr_network"].insert("boost_system");
		_marks_dep["yggr_network"].insert("boost_thread");
		_marks_dep["yggr_network"].insert("pthread");
		_marks_dep["yggr_network"].insert("ws2_32");

		_marks_dep["yggr_regular_parse"].insert("boost_regex");

		_marks_dep["yggr_segment"].insert("boost_regex");
		_marks_dep["yggr_segment"].insert("boost_filesystem");

		_marks_dep["yggr_packet"].insert("boost_wserialization");
		_marks_dep["yggr_packet"].insert("boost_serialization");
		_marks_dep["yggr_packet"].insert("boost_system");
		_marks_dep["yggr_packet"].insert("ws2_32");

		_marks_dep["yggr_file_system"].insert("boost_filesystem");
		_marks_dep["yggr_file_system"].insert("boost_regex");
		_marks_dep["yggr_file_system"].insert("Bcrypt");
		_marks_dep["yggr_file_system"].insert("ws2_32");

		_marks_dep["yggr_safe_container"].insert("ws2_32");
		
		_marks_dep["yggr_ids"].insert("Bcrypt");
		_marks_dep["yggr_exception"].insert("md5");

		_marks_dep["yggr_thread"].insert("boost_thread");
		_marks_dep["yggr_thread"].insert("boost_system");

		_marks_dep["yggr_math"].insert("ws2_32");

		_marks_dep["yggr_handler_center"].insert("ws2_32");
		_marks_dep["yggr_segment"].insert("ws2_32");
		_marks_dep["yggr_archive"].insert("ws2_32");

		_marks_dep["boost_asio.hpp"].insert("ws2_32");
		
		//yggr_include_marks end

		// vcproj file marks beg
		_marks_dep["protocol_reg"].insert("boost_serialization");
		_marks_dep["protocol_reg"].insert("boost_filesystem");
		_marks_dep["protocol_reg"].insert("boost_regex");
		_marks_dep["protocol_reg"].insert("boost_system");
		_marks_dep["protocol_reg"].insert("ws2_32");
		_marks_dep["protocol_reg"].insert("mswsock");
		_marks_dep["protocol_reg"].insert("Bcrypt");

		_marks_dep["vcxproj_parser_patch"].insert("ws2_32");
		// vcproj file marks end

		// root_namespace marks beg
		_marks_dep["bid_test"].insert("bid");
		_marks_dep["intel_dfp_obj_test"].insert("intel_dfp_obj");

		_marks_dep["dll_cal_test"].insert("boost_serialization");
		_marks_dep["dll_cal_test"].insert("boost_system");
		_marks_dep["dll_cal_test"].insert("ws2_32");
		_marks_dep["dll_cal_test"].insert("Bcrypt");

		_marks_dep["lua_active_svr_test"].insert("boost_serialization");
		_marks_dep["lua_active_svr_test"].insert("boost_filesystem");
		_marks_dep["lua_active_svr_test"].insert("boost_regex");
		_marks_dep["lua_active_svr_test"].insert("boost_system");
		_marks_dep["lua_active_svr_test"].insert("ws2_32");
		_marks_dep["lua_active_svr_test"].insert("mswsock");
		_marks_dep["lua_active_svr_test"].insert("Bcrypt");

		_marks_dep["active_svr_test"].insert("boost_serialization");
		_marks_dep["active_svr_test"].insert("boost_filesystem");
		_marks_dep["active_svr_test"].insert("boost_regex");
		_marks_dep["active_svr_test"].insert("boost_system");
		_marks_dep["active_svr_test"].insert("ws2_32");
		_marks_dep["active_svr_test"].insert("mswsock");
		_marks_dep["active_svr_test"].insert("Bcrypt");

		_marks_dep["lua_calculator"].insert("Bcrypt");

		_marks_dep["encryption_tool_clt_test"].insert("blowfish");
		_marks_dep["encryption_tool_clt_test"].insert("md5");

		_marks_dep["encryption_tool_proxy_srv_test"].insert("blowfish");
		_marks_dep["encryption_tool_proxy_srv_test"].insert("md5");

		_marks_dep["encryption_tool_proxy_trun_srv_test"].insert("blowfish");
		_marks_dep["encryption_tool_proxy_trun_srv_test"].insert("md5");

		_marks_dep["encryption_tool_svr_test"].insert("blowfish");
		_marks_dep["encryption_tool_svr_test"].insert("md5");

		_marks_dep["mixer_svr_test"].insert("md5");

		_marks_dep["p2p_boot_svr_test"].insert("boost_filesystem");
		_marks_dep["p2p_boot_svr_test"].insert("boost_regex");
		_marks_dep["p2p_boot_svr_test"].insert("md5");
		_marks_dep["p2p_boot_svr_test"].insert("Bcrypt");

		_marks_dep["p2p_peer_test"].insert("boost_filesystem");
		_marks_dep["p2p_peer_test"].insert("boost_regex");
		_marks_dep["p2p_peer_test"].insert("md5");
		_marks_dep["p2p_peer_test"].insert("Bcrypt");

		_marks_dep["proxy_trun_mixer_svr_test"].insert("md5");

		_marks_dep["geometry_geo_bezier_curve_gui_test"].insert("gdi32");

		// root_namespace marks end

		_marks_dep["pyd_export"].insert("boost_python");
		_marks_dep["pyd_export"].insert("python");

		_marks_dep["luabind"].insert("lua");

		_marks_dep["luabind_test"].insert("lua");
		_marks_dep["luabind_test"].insert("luabind09");

		_marks_dep["lua503"].insert("lua503");
		_marks_dep["lua502"].insert("lua502");
		_marks_dep["lua501"].insert("lua501");

		_marks_dep["lua503-s"].insert("lua503-s");
		_marks_dep["lua502-s"].insert("lua502-s");
		_marks_dep["lua501-s"].insert("lua501-s");

		_marks_dep["lua503_ts-s"].insert("lua503_ts-s");
		_marks_dep["lua502_ts-s"].insert("lua502_ts-s");
		_marks_dep["lua501_ts-s"].insert("lua501_ts-s");

		_marks_dep["script_lua"].insert("yggr_script_lua");
		_marks_dep["script_lua"].insert("luabind09");
		_marks_dep["script_lua"].insert("lua");
		_marks_dep["script_lua"].insert("ws2_32");

		_marks_dep["script_lua_s"].insert("yggr_script_lua");
		_marks_dep["script_lua_s"].insert("luabind09_s");
		_marks_dep["script_lua_s"].insert("lua");
		_marks_dep["script_lua_s"].insert("ws2_32");

		_marks_dep["script_lua_ss"].insert("yggr_script_lua");
		_marks_dep["script_lua_ss"].insert("luabind09_s");
		_marks_dep["script_lua_ss"].insert("lua_s");
		_marks_dep["script_lua_ss"].insert("ws2_32");

		_marks_dep["script_lua_ss_ts"].insert("yggr_script_lua");
		_marks_dep["script_lua_ss_ts"].insert("luabind09_ts");
		_marks_dep["script_lua_ss_ts"].insert("lua_ts");
		_marks_dep["script_lua_ss_ts"].insert("ws2_32");
		_marks_dep["script_lua_ss_ts"].insert("pthread");

		_marks_dep["script_python"].insert("yggr_script_python");
		_marks_dep["script_python"].insert("boost_thread");
		_marks_dep["script_python"].insert("python");

		_marks_dep["segment"].insert("yggr_segment");

		// mingw not support seh
		//_marks_dep["seh"].insert("yggr_seh");
		//_marks_dep["seh"].insert("dbghelp");

		_marks_dep["system_controller"].insert("yggr_system_controller");
		_marks_dep["system_controller"].insert("boost_thread");

		_marks_dep["time"].insert("yggr_time");
		_marks_dep["time"].insert("boost_date_time");

		_marks_dep["boost_system"].insert("boost_system");
		_marks_dep["boost_thread"].insert("boost_thread");
		_marks_dep["boost_thread"].insert("ws2_32");

		_marks_dep["boost_atomic"].insert("boost_atomic");
		_marks_dep["boost_date_time"].insert("boost_date_time");
		_marks_dep["boost_timer"].insert("boost_timer");

		_marks_dep["boost_serialization"].insert("boost_serialization");
		_marks_dep["boost_wserialization"].insert("boost_wserialization");
		_marks_dep["boost_filesystem"].insert("boost_filesystem");

		_marks_dep["boost_regex"].insert("boost_regex");
		
		_marks_dep["boost_python"].insert("boost_python");
		_marks_dep["boost_python"].insert("python");

		//_marks_dep["lua"].insert("lua");

		// icu not support mingw
		_marks_dep["ibm_icu"].insert("icuuc");
		_marks_dep["ibm_icu"].insert("icuin");

		_marks_dep["openssl"].insert("crypto");
		_marks_dep["openssl"].insert("ssl");

		_marks_dep["iconv"].insert("iconv");
		_marks_dep["sha"].insert("sha");
		_marks_dep["md5"].insert("md5");
		_marks_dep["base64"].insert("base64");
		_marks_dep["blowfish"].insert("blowfish");
		_marks_dep["zlib"].insert("zlib");
		_marks_dep["zlibstatic"].insert("zlibstatic");
		_marks_dep["dtl"].insert("dtl");

		_marks_dep["pthread"].insert("pthread");

		_marks_dep["Dnsapi"].insert("Dnsapi");
		//_marks_dep["Bcrypt"].insert("Bcrypt");
		//_marks_dep["Secur32"].insert("Secur32");
		//_marks_dep["Crypt32"].insert("Crypt32");
		_marks_dep["Bcrypt"].insert("crypto");
		_marks_dep["Bcrypt"].insert("ssl");
		_marks_dep["Secur32"].insert("crypto");
		_marks_dep["Secur32"].insert("ssl");
		_marks_dep["Crypt32"].insert("crypto");
		_marks_dep["Crypt32"].insert("ssl");

		_marks_dep["mgw_gcc_sjlj"].insert("mgw_gcc_sjlj");

		_marks_dep["ws2_32"].insert("ws2_32");
		_marks_dep["user32"].insert("user32");
		_marks_dep["kernel32"].insert("kernel32");

		_marks_dep["d3d9"].insert("d3d9");
		_marks_dep["d3dx9"].insert("d3dx9");
		_marks_dep["Winmm"].insert("winmm");
		_marks_dep["winmm"].insert("winmm");
	}

	// marks_level
	{

		_marks_lv["gdi32"] = 0;
		_marks_lv["user32"] = 0;
		_marks_lv["kernel32"] = 0;

		_marks_lv["winmm"] = 1;
		_marks_lv["Dnsapi"] = 1;
		_marks_lv["Bcrypt"] = 1;
		_marks_lv["Secur32"] = 1;
		_marks_lv["Crypt32"] = 1;

		_marks_lv["ws2_32"] = 2;
		_marks_lv["mswsock"] = 3;
		_marks_lv["odbc32"] = 3;
		_marks_lv["wsock32"] = 4;
		_marks_lv["lzma"] = 5;

		_marks_lv["d3d9"] = 5;
		_marks_lv["d3dx9"] = 5;

		_marks_lv["mgw_gcc_sjlj"] = 6;

		_marks_lv["pthread"] = 9;

		_marks_lv["iconv"] = 10;
		_marks_lv["sha"] = 10;
		_marks_lv["md5"] = 10;
		_marks_lv["base64"] = 10;
		_marks_lv["blowfish"] = 10;
		_marks_lv["zlib"] = 10;
		_marks_lv["zlibstatic"] = 10;
		_marks_lv["dtl"] = 10;

		_marks_lv["bid"] = 10; // nsql_database_system
		_marks_lv["intel_dfp_obj"] = 10; // nsql_database_system
		_marks_lv["sasl"] = 10; // nsql_database_system

		_marks_lv["crypto"] = 10;
		_marks_lv["ssl"] = 10;

		_marks_lv["icuuc"] = 12;
		_marks_lv["icuin"] = 12;

		_marks_lv["python"] = 13;

		_marks_lv["lua"] = 13;
		_marks_lv["lua_s"] = 13;
		_marks_lv["lua_ts"] = 13;

		_marks_lv["lua503"] = 13;
		_marks_lv["lua502"] = 13;
		_marks_lv["lua501"] = 13;

		_marks_lv["lua503-s"] = 13;
		_marks_lv["lua502-s"] = 13;
		_marks_lv["lua501-s"] = 13;

		_marks_lv["lua503_ts-s"] = 13;
		_marks_lv["lua502_ts-s"] = 13;
		_marks_lv["lua501_ts-s"] = 13;

		_marks_lv["luabind09"] = 14;
		_marks_lv["luabind09_s"] = 14;
		_marks_lv["luabind09_ts"] = 14;

		_marks_lv["common"] = 50;
		_marks_lv["bson"] = 51;
		_marks_lv["utf8proc"] = 52;
		_marks_lv["kms-message"] = 53;
		_marks_lv["mongocrypt"] = 54;
		_marks_lv["mongoc"] = 55;

		_marks_lv["boost_system"] = 500;
		_marks_lv["boost_thread"] = 502;

		_marks_lv["boost_atomic"] = 503;
		_marks_lv["boost_date_time"] = 504;
		_marks_lv["boost_timer"] = 505;

		_marks_lv["boost_serialization"] = 600;
		_marks_lv["boost_wserialization"] = 601;
		_marks_lv["boost_filesystem"] = 602;
		_marks_lv["boost_regex"] = 603;
		_marks_lv["boost_python"] = 604;

		_marks_lv["yggr_compatibility"] = 1000;
		_marks_lv["yggr_base"] = 1001;
		_marks_lv["yggr_charset"] = 1002;
		_marks_lv["yggr_time"] = 1003;
		_marks_lv["yggr_ids"] = 1004;
		_marks_lv["yggr_compaction_tool"] = 1005;
		_marks_lv["yggr_encryption_tool"] = 1006;
		_marks_lv["yggr_exception"] = 1007;
		_marks_lv["yggr_system_controller"] = 1008;
		_marks_lv["yggr_seh"] = 1009;
		_marks_lv["yggr_modules"] = 1010;

		_marks_lv["yggr_segment"] = 1011;
		_marks_lv["yggr_log"] = 1012;
		_marks_lv["yggr_database_system"] = 1013;
		_marks_lv["yggr_nsql_database_system"] = 1014;

		_marks_lv["yggr_any_val"] = 1015;
		//_marks_lv["yggr_any_val_boost"] = 1015;
		_marks_lv["yggr_script_lua"] = 1016;
		_marks_lv["yggr_script_python"] = 1017;
	}

	// marks_name
	// marks_level
	// %(debug) // -d or nil
	// %(sdbug) // -sd or -s
	// %(lua_version) // 501 502 503
	// %(addr_model) // -x32 -x64
	// %(python_main_version)
	{
		_marks_name["gdi32"] = "-lgdi32";
		_marks_name["ws2_32"] = "-lws2_32";
		_marks_name["user32"] = "-luser32";
		_marks_name["kernel32"] = "-lkernel32";
		_marks_name["odbc32"] = "-lodbc32";
		_marks_name["wsock32"] = "-lwsock32";
		_marks_name["mswsock"] = "-lmswsock";

		_marks_name["d3d9"] = "-ld3d9";
		_marks_name["d3dx9"] = "-ld3dx9";
		_marks_name["winmm"] = "-lwinmm";

		_marks_name["Dnsapi"] = "-ldnsapi";
		_marks_name["Bcrypt"] = "-lbcrypt";
		_marks_name["Secur32"] = "-lsecur32";
		_marks_name["Crypt32"] = "-lcrypt32";

		_marks_name["lzma"] = "-llzma$(#COMPILER_VERSION_TAG)%(debug).dll";
		_marks_name["pthread"] = "-lpthread";

		_marks_name["mgw_gcc_sjlj"] = "-lmgw_gcc_sjlj$(#COMPILER_VERSION_TAG)%(debug)";

		_marks_name["iconv"] = "-liconv";
		_marks_name["sha"] = "-lsha$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["md5"] = "-lmd5$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["base64"] = "-lbase64$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["blowfish"] = "-lblowfish$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["zlib"] = "-lz$(#COMPILER_VERSION_TAG)%(debug).dll";
		_marks_name["zlibstatic"] = "-lz$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["dtl"] = "-ldtl$(#COMPILER_VERSION_TAG)%(debug)";

		_marks_name["crypto"] = "-lcrypto";
		_marks_name["ssl"] = "-lssl";
		_marks_name["sasl"] = "-lsasl2.dll";

		_marks_name["icuuc"] = "-licuuc";
		_marks_name["icuin"] = "-licuin";

		_marks_name["python"] = "-lpython%(python_main_version)";

		_marks_name["lua"] = "-llua%(lua_version)$(#COMPILER_VERSION_TAG)%(debug).dll";
		_marks_name["lua_s"] = "-llua%(lua_version)$(#COMPILER_VERSION_TAG)-s%(debug)";
		_marks_name["lua_ts"] = "-llua%(lua_version)_ts$(#COMPILER_VERSION_TAG)-s%(debug)";

		_marks_name["lua503"] = "-llua503$(#COMPILER_VERSION_TAG)%(debug).dll";
		_marks_name["lua502"] = "-llua502$(#COMPILER_VERSION_TAG)%(debug).dll";
		_marks_name["lua501"] = "-llua501$(#COMPILER_VERSION_TAG)%(debug).dll";

		_marks_name["lua503-s"] = "-llua503$(#COMPILER_VERSION_TAG)-s%(debug)";
		_marks_name["lua502-s"] = "-llua502$(#COMPILER_VERSION_TAG)-s%(debug)";
		_marks_name["lua501-s"] = "-llua501$(#COMPILER_VERSION_TAG)-s%(debug)";

		_marks_name["lua503_ts-s"] = "-llua503_ts$(#COMPILER_VERSION_TAG)-s%(debug)";
		_marks_name["lua502_ts-s"] = "-llua502_ts$(#COMPILER_VERSION_TAG)-s%(debug)";
		_marks_name["lua501_ts-s"] = "-llua501_ts$(#COMPILER_VERSION_TAG)-s%(debug)";

		_marks_name["luabind09"] = "-lluabind09$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["luabind09_s"] = "-lluabind09$(#COMPILER_VERSION_TAG)-s%(debug)";
		_marks_name["luabind09_ts"] = "-lluabind09_ts$(#COMPILER_VERSION_TAG)-s%(debug)";

		// boost
		_marks_name["boost_system"] = "-lboost_system$(#COMPILER_VERSION_TAG)-mt%(debug)%(addr_model)$(#BOOST_VERSION_TAG)";
		_marks_name["boost_thread"] = "-lboost_thread$(#COMPILER_VERSION_TAG)-mt%(debug)%(addr_model)$(#BOOST_VERSION_TAG)";

		_marks_name["boost_atomic"] = "-lboost_atomic$(#COMPILER_VERSION_TAG)-mt%(debug)%(addr_model)$(#BOOST_VERSION_TAG)";
		_marks_name["boost_date_time"] = "-lboost_date_time$(#COMPILER_VERSION_TAG)-mt%(debug)%(addr_model)$(#BOOST_VERSION_TAG)";
		_marks_name["boost_timer"] = "-lboost_timer$(#COMPILER_VERSION_TAG)-mt%(debug)%(addr_model)$(#BOOST_VERSION_TAG)";

		_marks_name["boost_serialization"] = "-lboost_serialization$(#COMPILER_VERSION_TAG)-mt%(sdebug)%(addr_model)$(#BOOST_VERSION_TAG)";
		_marks_name["boost_wserialization"] = "-lboost_wserialization$(#COMPILER_VERSION_TAG)-mt%(sdebug)%(addr_model)$(#BOOST_VERSION_TAG)";
		_marks_name["boost_filesystem"] = "-lboost_filesystem$(#COMPILER_VERSION_TAG)-mt%(debug)%(addr_model)$(#BOOST_VERSION_TAG)";
		_marks_name["boost_regex"] = "-lboost_regex$(#COMPILER_VERSION_TAG)-mt%(debug)%(addr_model)$(#BOOST_VERSION_TAG)";
		_marks_name["boost_python"] = "-lboost_python$(#PYTHON_VERSION)$(#COMPILER_VERSION_TAG)-mt%(debug)%(addr_model)$(#BOOST_VERSION_TAG)";

		_marks_name["yggr_compatibility"] = "-lyggr_compatibility$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["yggr_base"] = "-lyggr_base$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["yggr_charset"] = "-lyggr_charset$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["yggr_time"] = "-lyggr_time$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["yggr_ids"] = "-lyggr_ids$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["yggr_compaction_tool"] = "-lyggr_compaction_tool$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["yggr_encryption_tool"] = "-lyggr_encryption_tool$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["yggr_exception"] = "-lyggr_exception$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["yggr_system_controller"] = "-lyggr_system_controller$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["yggr_modules"] = "-lyggr_modules$(#COMPILER_VERSION_TAG)%(debug)";

		_marks_name["yggr_segment"] = "-lyggr_segment$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["yggr_log"] = "-lyggr_log$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["yggr_database_system"] = "-lyggr_database_system$(#COMPILER_VERSION_TAG)%(debug)";

		_marks_name["yggr_nsql_database_system"] = "-lyggr_nsql_database_system$(#COMPILER_VERSION_TAG)%(debug)";

		_marks_name["yggr_any_val"] = "-lyggr_any_val$(#COMPILER_VERSION_TAG)%(debug)";
		//_marks_name["yggr_any_val_boost"] = "-lyggr_any_val_boost$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["yggr_script_lua"] = "-lyggr_script_lua$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["yggr_script_python"] = "-lyggr_script_python$(#COMPILER_VERSION_TAG)%(debug)";

		_marks_name["yggr_seh"] = "-lyggr_seh$(#COMPILER_VERSION_TAG)%(debug)";

		_marks_name["mongoc"] = "-lmongoc$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["bson"] = "-lbson$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["common"] = "-lcommon$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["mongocrypt"] = "-lmongocrypt$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["kms-message"] = "-lkms-message$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["utf8proc"] = "-lutf8proc$(#COMPILER_VERSION_TAG)%(debug)";

		_marks_name["intel_dfp_obj"] = "-lintel_dfp_obj$(#COMPILER_VERSION_TAG)%(debug)";
		_marks_name["bid"] = "-lbid$(#COMPILER_VERSION_TAG)%(debug)";

	}
}

lib_depend_info::lib_depend_info(const this_type& right)
	: _str_boost_ver(right._str_boost_ver),
		_str_lua_ver(right._str_lua_ver),
		_str_python_ver(right._str_python_ver),
		_marks_dep(right._marks_dep),
		_marks_lv(right._marks_lv),
		_marks_name(right._marks_name)
{
}


lib_depend_info::~lib_depend_info(void)
{
}

lib_depend_info::this_type& lib_depend_info::operator=(const this_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	_str_boost_ver = right._str_boost_ver;
	_str_lua_ver = right._str_lua_ver;
	_str_python_ver = right._str_python_ver;

	_marks_dep = right._marks_dep;
	_marks_lv = right._marks_lv;
	_marks_name = right._marks_name;

	return *this;
}

void lib_depend_info::swap(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return;
	}

	_str_boost_ver.swap(right._str_boost_ver);
	_str_lua_ver.swap(right._str_lua_ver);
	_str_python_ver.swap(right._str_python_ver);

	_marks_dep.swap(right._marks_dep);
	_marks_lv.swap(right._marks_lv);
	_marks_name.swap(right._marks_name);
}

string_set_type& lib_depend_info::format_libmarks(string_set_type& out,
													const string_set_type& libmarks) const
{
	typedef mark_depend_mapping_type::const_iterator dep_citer_type;

	dep_citer_type dep_citer;

	for(string_set_type::const_iterator i = libmarks.begin(), isize = libmarks.end(); i != isize; ++i)
	{
		if((dep_citer = _marks_dep.find(*i)) != _marks_dep.end())
		{
			out.insert(dep_citer->second.begin(), dep_citer->second.end());
		}
	}

	return out;
}

lib_depend_info::ordered_string_map_type&
	lib_depend_info::sort_libmarks(ordered_string_map_type& out,
									const string_set_type& libmarks) const
{

	//static const yggr::s32 min_lv = -(0x7fffffff);
	static const yggr::s32 max_lv = 0x7fffffff;


	typedef mark_level_mapping_type::const_iterator lv_citer_type;

	lv_citer_type lv_citer;

	for(string_set_type::const_iterator i = libmarks.begin(), isize = libmarks.end(); i != isize; ++i)
	{
		if((lv_citer = _marks_lv.find(*i)) != _marks_lv.end())
		{
			out.insert(std::make_pair(lv_citer->second, lv_citer->first));
		}
		else
		{
			std::cout << "unknow libmarks level: " << *i << std::endl;
			assert(false);
			out.insert(std::make_pair(max_lv, *i));
		}
	}

	return out;
}

lib_depend_info::string_type
	lib_depend_info::libmark_to_libname(const string_type& libmark,
										const string_type& vc_cfg_mark,
										const string_type& str_boost_ver,
										const string_type& str_lua_ver,
										const string_type& str_python_ver) const
{
	typedef string_type::iterator string_iter_type;
	typedef string_type::const_iterator string_citer_type;

	// %(debug) // -d or nil
	// %(sdebug) // -sd or -s
	// %(lua_version) // 501 502 503
	// %(addr_model) // -x32 -x64
	// %(python_main_version)

	static const string_type debug_mark = "%(debug)";
	static const string_type debug2_mark = "%(sdebug)";
	static const string_type addr_model_mark = "%(addr_model)";
	static const string_type lua_ver_mark = "%(lua_version)";
	static const string_type python_ver_mark = "%(python_main_version)";

	static const string_type str_debug_tag = "-d";
	static const string_type str_debug2_tag = "-sd";
	static const string_type str_release2_tag = "-s";
	static const string_type str_addr_model_32 = "-x32";
	static const string_type str_addr_model_64 = "-x64";

	if(str_boost_ver.size() < 4
		|| str_lua_ver.size() < 3
		|| str_python_ver.size() < 2)
	{
		assert(false);
		return string_type();
	}

	string_type tmp_str = libmark;

	// debug_mark
	{
		std::size_t pos = tmp_str.find(debug_mark);
		if(pos != string_type::npos)
		{
			if(vcproj::vcproj_cfg_marks::s_is_debug_mark(vc_cfg_mark))
			{
				tmp_str.replace(tmp_str.begin() + pos, tmp_str.begin() + (pos + debug_mark.size()), str_debug_tag);
			}
			else if(vcproj::vcproj_cfg_marks::s_is_release_mark(vc_cfg_mark))
			{
				tmp_str.erase(tmp_str.begin() + pos, tmp_str.begin() + (pos + debug_mark.size()));
			}
			else
			{
				return string_type();
			}
		}
	}

	// debug2_mark
	{
		std::size_t pos = tmp_str.find(debug2_mark);
		if(pos != string_type::npos)
		{
			if(vcproj::vcproj_cfg_marks::s_is_debug_mark(vc_cfg_mark))
			{
				tmp_str.replace(tmp_str.begin() + pos, tmp_str.begin() + (pos + debug2_mark.size()), str_debug2_tag);
			}
			else if(vcproj::vcproj_cfg_marks::s_is_release_mark(vc_cfg_mark))
			{
				tmp_str.replace(tmp_str.begin() + pos, tmp_str.begin() + (pos + debug2_mark.size()), str_release2_tag);
			}
			else
			{
				return string_type();
			}
		}
	}

	// address_mode_mark
	{
		std::size_t pos = tmp_str.find(addr_model_mark);
		if(pos != string_type::npos)
		{
			if(vcproj::vcproj_cfg_marks::s_is_x32_mark(vc_cfg_mark))
			{
				tmp_str.replace(tmp_str.begin() + pos, tmp_str.begin() + (pos + addr_model_mark.size()), str_addr_model_32);
			}
			else if(vcproj::vcproj_cfg_marks::s_is_x64_mark(vc_cfg_mark))
			{
				tmp_str.replace(tmp_str.begin() + pos , tmp_str.begin() + (pos + addr_model_mark.size()), str_addr_model_64);
			}
			else
			{
				return string_type();
			}
		}
	}

	// lua_ver_mark
	{
		std::size_t pos = tmp_str.find(lua_ver_mark);
		if(pos != string_type::npos)
		{
			tmp_str.replace(tmp_str.begin() + pos, tmp_str.begin() + (pos + lua_ver_mark.size()), str_lua_ver);
		}
	}

	//python_ver_mark
	{
		std::size_t pos = tmp_str.find(python_ver_mark);
		if(pos != string_type::npos)
		{
			tmp_str.replace(tmp_str.begin() + pos , tmp_str.begin() + (pos + python_ver_mark.size()), str_python_ver);
		}
	}

	return tmp_str;
}

lib_depend_info::ordered_string_map_type&
	lib_depend_info::libmarks_to_libnames(ordered_string_map_type& inout,
											const string_type& vc_cfg_mark,
											const string_type& str_boost_ver,
											const string_type& str_lua_ver,
											const string_type& str_python_ver) const
{
	typedef mark_name_mapping_type::const_iterator mark_name_citer_type;

	for(ordered_string_map_type::iterator i = inout.begin(), isize= inout.end(); i != isize; ++i)
	{
		mark_name_citer_type iter_name_tpl = _marks_name.find(i->second);

		if(iter_name_tpl != _marks_name.end())
		{
			i->second = libmark_to_libname(iter_name_tpl->second, vc_cfg_mark, str_boost_ver, str_lua_ver, str_python_ver);
		}
	}

	return inout;
}

} // namespace lib

#endif // !(BOOST_VERSION < 105600)
