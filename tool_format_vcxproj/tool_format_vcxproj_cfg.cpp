// tool_format_vcxproj_cfg.cpp

#include "tool_format_vcxproj_cfg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#if !(BOOST_VERSION < 105600)

tool_format_vcxproj_cfg::tool_format_vcxproj_cfg(void)
{
}

tool_format_vcxproj_cfg::tool_format_vcxproj_cfg(tool_format_vcxproj_cfg_sample)
	: root_dir_(".")
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
	dir_filter_.insert("Debug-MT-Win32");
	dir_filter_.insert("Debug-MT-x64");
	dir_filter_.insert("Release-MT-Win32");
	dir_filter_.insert("Release-MT-x64");
	dir_filter_.insert("jni");
	dir_filter_.insert(".git");
	dir_filter_.insert(".vs");
	dir_filter_.insert(".vscode");
	dir_filter_.insert("discard");
	dir_filter_.insert("licenses");
	dir_filter_.insert("need_reconstruction");
	dir_filter_.insert("yggdrasil-ios.xcworkspace");


	//file_filter_.insert("file_sample1");
	//file_filter_.insert("file_sample2");

	proj_name_map_["lua514"] = "lua501-vc$(PlatformToolsetVersion)";
	proj_name_map_["lua514_ts"] = "lua501_ts-vc$(PlatformToolsetVersion)";
	proj_name_map_["lua514_static"] = "liblua501_static-vc$(PlatformToolsetVersion)";
	proj_name_map_["lua514_ts_static"] = "liblua501_ts_static-vc$(PlatformToolsetVersion)";

	proj_name_map_["lua521"] = "lua502-vc$(PlatformToolsetVersion)";
	proj_name_map_["lua521_ts"] = "lua502_ts-vc$(PlatformToolsetVersion)";
	proj_name_map_["lua521_static"] = "liblua502_static-vc$(PlatformToolsetVersion)";
	proj_name_map_["lua521_ts_static"] = "liblua502_ts_static-vc$(PlatformToolsetVersion)";

	proj_name_map_["lua534"] = "lua503-vc$(PlatformToolsetVersion)";
	proj_name_map_["lua534_ts"] = "lua503_ts-vc$(PlatformToolsetVersion)";
	proj_name_map_["lua534_static"] = "liblua503_static-vc$(PlatformToolsetVersion)";
	proj_name_map_["lua534_ts_static"] = "liblua503_ts_static-vc$(PlatformToolsetVersion)";

	proj_name_map_["lua546"] = "lua504-vc$(PlatformToolsetVersion)";
	proj_name_map_["lua546_ts"] = "lua504_ts-vc$(PlatformToolsetVersion)";
	proj_name_map_["lua546_static"] = "liblua504_static-vc$(PlatformToolsetVersion)";
	proj_name_map_["lua546_ts_static"] = "liblua504_ts_static-vc$(PlatformToolsetVersion)";

	proj_name_map_["luabind"] = "luabind09-vc$(PlatformToolsetVersion)";
	proj_name_map_["luabind_ts"] = "luabind09_ts-vc$(PlatformToolsetVersion)";
	proj_name_map_["luabind_static"] = "libluabind09_static-vc$(PlatformToolsetVersion)";
	proj_name_map_["luabind_ts_static"] = "libluabind09_ts_static-vc$(PlatformToolsetVersion)";
	
	proj_name_map_["pyd_export"] = "@pyst#.pyd";
	proj_name_map_["dll_cal_test"] = "@dll_cal_test";

	proj_name_map_["yggr_lua_base_type"] = "@yggr_lua_base_type";
	proj_name_map_["lua_cal_svr_procotol"] = "@lua_cal_svr_procotol";
	proj_name_map_["lua_hello_dll"] = "@lua_hello_dll";
	proj_name_map_["lua_calculator"] = "@lua_calculator";

	proj_name_map_["LzmaLib"] = "lzma-vc$(PlatformToolsetVersion)";
	proj_name_map_["LzmaLib_static"] = "liblzma_static-vc$(PlatformToolsetVersion)";
}

tool_format_vcxproj_cfg::tool_format_vcxproj_cfg(const this_type& right)
	: root_dir_(right.root_dir_),
		dir_filter_(right.dir_filter_),
		file_filter_(right.file_filter_),
		proj_name_map_(right.proj_name_map_)
{
}

tool_format_vcxproj_cfg::~tool_format_vcxproj_cfg(void)
{
}

tool_format_vcxproj_cfg::this_type& 
	tool_format_vcxproj_cfg::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	root_dir_ = right.root_dir_;
	dir_filter_ = right.dir_filter_;
	file_filter_ = right.file_filter_;
	proj_name_map_ = right.proj_name_map_;

	return *this;
}

void tool_format_vcxproj_cfg::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	yggr::swap(root_dir_, right.root_dir_);
	yggr::swap(dir_filter_, right.dir_filter_);
	yggr::swap(file_filter_, right.file_filter_);
	yggr::swap(proj_name_map_, right.proj_name_map_);
}

namespace swap_support
{
	YGGR_PP_SWAP_THIS_REF_DEF_IMPL(tool_format_vcxproj_cfg)
} // namespace swap_support

#endif // #if !(BOOST_VERSION < 105600)