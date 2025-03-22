// tool_conv_to_cbsln_cfg.cpp

#include "tool_conv_to_cbsln_cfg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#if !(BOOST_VERSION < 105600)

tool_conv_to_cb_solution_cfg::tool_conv_to_cb_solution_cfg(void)
{
}

tool_conv_to_cb_solution_cfg::tool_conv_to_cb_solution_cfg(tool_conv_to_cbsln_cfg_sample)
	: cbp_sample_cfg_("cbp_sample_cfg.xml"),
		lib_depend_info_cfg_("lib_depend_info_cfg.xml"),
		root_dir_(".")
{
	dir_filter_.insert(".");
	dir_filter_.insert("dir_sample1");
	dir_filter_.insert("dir_sample2");

	file_filter_.insert("file_sample1");
	file_filter_.insert("file_sample2");
}

tool_conv_to_cb_solution_cfg::tool_conv_to_cb_solution_cfg(const this_type& right)
	: cbp_sample_cfg_(right.cbp_sample_cfg_),
		lib_depend_info_cfg_(right.lib_depend_info_cfg_),
		root_dir_(right.root_dir_),
		dir_filter_(right.dir_filter_),
		file_filter_(right.file_filter_)
{
}

tool_conv_to_cb_solution_cfg::~tool_conv_to_cb_solution_cfg(void)
{
}

tool_conv_to_cb_solution_cfg::this_type& 
	tool_conv_to_cb_solution_cfg::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	cbp_sample_cfg_ = right.cbp_sample_cfg_;
	lib_depend_info_cfg_ = right.lib_depend_info_cfg_;
	root_dir_ = right.root_dir_;
	dir_filter_ = right.dir_filter_;
	file_filter_ = right.file_filter_;

	return *this;
}

void tool_conv_to_cb_solution_cfg::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	yggr::swap(cbp_sample_cfg_, right.cbp_sample_cfg_);
	yggr::swap(lib_depend_info_cfg_, right.lib_depend_info_cfg_);
	yggr::swap(root_dir_, right.root_dir_);
	yggr::swap(dir_filter_, right.dir_filter_);
	yggr::swap(file_filter_, right.file_filter_);
}

namespace swap_support
{
	YGGR_PP_SWAP_THIS_REF_DEF_IMPL(tool_conv_to_cb_solution_cfg)
} // namespace swap_support

#endif // #if !(BOOST_VERSION < 105600)