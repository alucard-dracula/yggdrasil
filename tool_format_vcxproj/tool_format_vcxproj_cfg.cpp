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
	dir_filter_.insert(".");
	dir_filter_.insert("dir_sample1");
	dir_filter_.insert("dir_sample2");

	file_filter_.insert("file_sample1");
	file_filter_.insert("file_sample2");

	proj_name_map_["not_exists_key_sample1"] = "not_exists_mapping_sample1";
	proj_name_map_["not_exists_key_sample2"] = "not_exists_mapping_sample2";
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