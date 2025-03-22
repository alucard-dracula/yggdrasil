// test_project_maker_cfg.cpp

#include "test_project_maker_cfg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER


test_project_maker_cfg::test_project_maker_cfg(void)
{
}

test_project_maker_cfg::test_project_maker_cfg(test_project_maker_cfg_sample)
	: root_dir_("."),
		regular_sln_fname_("../yggr_test_only.sln"),
		test_bat_fname_("../run_test"),
		sample_sln_begin_("sample_begin.sln.smp"),
		sample_sln_end_("sample_end.sln.smp"),
		sample_sln_proj_begin_("sample_proj_begin.sln.smp"),
		sample_sln_proj_end_("sample_proj_end.sln.smp"),
		sample_sln_smp_dir_("smp_dir"),
		sample_sln_smp_proj_("smp_proj"),
		sample_sln_smp_id_("smp_id"),
		sample_vcxproj_("sample.vcxproj.smp"),
		sample_vcxporj_filter_("sample.vcxproj.filters.smp"),
		sample_vcxproj_re_("sample"),
		sample_vcxproj_fre_("smp_file.cpp")
{
	dir_filter_.insert(".");
	dir_filter_.insert("dir_sample1");
	dir_filter_.insert("dir_sample2");

	file_filter_.insert("file_sample1");
	file_filter_.insert("file_sample2");
}

test_project_maker_cfg::test_project_maker_cfg(const this_type& right)
	: root_dir_(right.root_dir_),
		regular_sln_fname_(right.regular_sln_fname_),
		test_bat_fname_(right.test_bat_fname_),
		sample_sln_begin_(right.sample_sln_begin_),
		sample_sln_end_(right.sample_sln_end_),
		sample_sln_proj_begin_(right.sample_sln_proj_begin_),
		sample_sln_proj_end_(right.sample_sln_proj_end_),
		sample_sln_smp_dir_(right.sample_sln_smp_dir_),
		sample_sln_smp_proj_(right.sample_sln_smp_proj_),
		sample_sln_smp_id_(right.sample_sln_smp_id_),
		sample_vcxproj_(right.sample_vcxproj_),
		sample_vcxporj_filter_(right.sample_vcxporj_filter_),
		sample_vcxproj_re_(right.sample_vcxproj_re_),
		sample_vcxproj_fre_(right.sample_vcxproj_fre_),
		dir_filter_(right.dir_filter_),
		file_filter_(right.file_filter_)
{
}

test_project_maker_cfg::~test_project_maker_cfg(void)
{
}

test_project_maker_cfg::this_type& 
	test_project_maker_cfg::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	root_dir_ = right.root_dir_;
	regular_sln_fname_ = right.regular_sln_fname_;
	test_bat_fname_ = right.test_bat_fname_;
	sample_sln_begin_ = right.sample_sln_begin_;
	sample_sln_end_ = right.sample_sln_end_;
	sample_sln_proj_begin_ = right.sample_sln_proj_begin_;
	sample_sln_proj_end_ = right.sample_sln_proj_end_;
	sample_sln_smp_dir_ = right.sample_sln_smp_dir_;
	sample_sln_smp_proj_ = right.sample_sln_smp_proj_;
	sample_sln_smp_id_ = right.sample_sln_smp_id_;
	sample_vcxproj_ = right.sample_vcxproj_;
	sample_vcxporj_filter_ = right.sample_vcxporj_filter_;
	sample_vcxproj_re_ = right.sample_vcxproj_re_;
	sample_vcxproj_fre_ = right.sample_vcxproj_fre_;
	dir_filter_ = right.dir_filter_;
	file_filter_ = right.file_filter_;

	return *this;
}

void test_project_maker_cfg::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	yggr::swap(root_dir_, right.root_dir_);
	yggr::swap(regular_sln_fname_, right.regular_sln_fname_);
	yggr::swap(test_bat_fname_, right.test_bat_fname_);
	yggr::swap(sample_sln_begin_, right.sample_sln_begin_);
	yggr::swap(sample_sln_end_, right.sample_sln_end_);
	yggr::swap(sample_sln_proj_begin_, right.sample_sln_proj_begin_);
	yggr::swap(sample_sln_proj_end_, right.sample_sln_proj_end_);
	yggr::swap(sample_sln_smp_dir_, right.sample_sln_smp_dir_);
	yggr::swap(sample_sln_smp_proj_, right.sample_sln_smp_proj_);
	yggr::swap(sample_sln_smp_id_, right.sample_sln_smp_id_);
	yggr::swap(sample_vcxproj_, right.sample_vcxproj_);
	yggr::swap(sample_vcxporj_filter_, right.sample_vcxporj_filter_);
	yggr::swap(sample_vcxproj_re_, right.sample_vcxproj_re_);
	yggr::swap(sample_vcxproj_fre_, right.sample_vcxproj_fre_);
	yggr::swap(dir_filter_, right.dir_filter_);
	yggr::swap(file_filter_, right.file_filter_);
}

namespace swap_support
{
	YGGR_PP_SWAP_THIS_REF_DEF_IMPL(test_project_maker_cfg)
} // namespace swap_support
