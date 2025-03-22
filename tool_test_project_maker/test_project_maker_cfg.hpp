// test_project_maker_cfg.hpp

#ifndef __TEST_PROJECT_MAKER_CFG_HPP__
#define __TEST_PROJECT_MAKER_CFG_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/serialization/access.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/container/unordered_set.hpp>

#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/unordered_set.hpp>


struct test_project_maker_cfg_sample {};

struct test_project_maker_cfg
{
public:
	typedef yggr::utf8_string string_type;
	typedef yggr::unordered_set<string_type> filter_list_type;

private:
	typedef test_project_maker_cfg this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type);

public:
	test_project_maker_cfg(void);
	test_project_maker_cfg(test_project_maker_cfg_sample);

	test_project_maker_cfg(BOOST_RV_REF(this_type) right)
		: root_dir_(boost::move(right.root_dir_)),
			regular_sln_fname_(boost::move(right.regular_sln_fname_)),
			test_bat_fname_(boost::move(right.test_bat_fname_)),
			sample_sln_begin_(boost::move(right.sample_sln_begin_)),
			sample_sln_end_(boost::move(right.sample_sln_end_)),
			sample_sln_proj_begin_(boost::move(right.sample_sln_proj_begin_)),
			sample_sln_proj_end_(boost::move(right.sample_sln_proj_end_)),
			sample_sln_smp_dir_(boost::move(right.sample_sln_smp_dir_)),
			sample_sln_smp_proj_(boost::move(right.sample_sln_smp_proj_)),
			sample_sln_smp_id_(boost::move(right.sample_sln_smp_id_)),
			sample_vcxproj_(boost::move(right.sample_vcxproj_)),
			sample_vcxporj_filter_(boost::move(right.sample_vcxporj_filter_)),
			sample_vcxproj_re_(boost::move(right.sample_vcxproj_re_)),
			sample_vcxproj_fre_(boost::move(right.sample_vcxproj_fre_)),
			dir_filter_(boost::move(right.dir_filter_)),
			file_filter_(boost::move(right.file_filter_))
	{
	}

	test_project_maker_cfg(const this_type& right);
	~test_project_maker_cfg(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;

		if(this == &right_ref)
		{
			return *this;
		}

		yggr::copy_or_move_or_swap(root_dir_, boost::move(right_ref.root_dir_));
		yggr::copy_or_move_or_swap(regular_sln_fname_, boost::move(right_ref.regular_sln_fname_));
		yggr::copy_or_move_or_swap(test_bat_fname_, boost::move(right_ref.test_bat_fname_));
		yggr::copy_or_move_or_swap(sample_sln_begin_, boost::move(right_ref.sample_sln_begin_));
		yggr::copy_or_move_or_swap(sample_sln_end_, boost::move(right_ref.sample_sln_end_));
		yggr::copy_or_move_or_swap(sample_sln_proj_begin_, boost::move(right_ref.sample_sln_proj_begin_));
		yggr::copy_or_move_or_swap(sample_sln_proj_end_, boost::move(right_ref.sample_sln_proj_end_));
		yggr::copy_or_move_or_swap(sample_sln_smp_dir_, boost::move(right_ref.sample_sln_smp_dir_));
		yggr::copy_or_move_or_swap(sample_sln_smp_proj_, boost::move(right_ref.sample_sln_smp_proj_));
		yggr::copy_or_move_or_swap(sample_sln_smp_id_, boost::move(right_ref.sample_sln_smp_id_));
		yggr::copy_or_move_or_swap(sample_vcxproj_, boost::move(right_ref.sample_vcxproj_));
		yggr::copy_or_move_or_swap(sample_vcxporj_filter_, boost::move(right_ref.sample_vcxporj_filter_));
		yggr::copy_or_move_or_swap(sample_vcxproj_re_, boost::move(right_ref.sample_vcxproj_re_));
		yggr::copy_or_move_or_swap(sample_vcxproj_fre_, boost::move(right_ref.sample_vcxproj_fre_));
		yggr::copy_or_move_or_swap(dir_filter_, boost::move(right_ref.dir_filter_));
		yggr::copy_or_move_or_swap(file_filter_, boost::move(right_ref.file_filter_));

		return *this;
	}

	this_type& operator=(const this_type& right);

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(root_dir_);
		ar & YGGR_SERIALIZATION_NVP(regular_sln_fname_);
		ar & YGGR_SERIALIZATION_NVP(test_bat_fname_);

		ar & YGGR_SERIALIZATION_NVP(sample_sln_begin_);
		ar & YGGR_SERIALIZATION_NVP(sample_sln_end_);

		ar & YGGR_SERIALIZATION_NVP(sample_sln_proj_begin_);
		ar & YGGR_SERIALIZATION_NVP(sample_sln_proj_end_);

		ar & YGGR_SERIALIZATION_NVP(sample_sln_smp_dir_);
		ar & YGGR_SERIALIZATION_NVP(sample_sln_smp_proj_);
		ar & YGGR_SERIALIZATION_NVP(sample_sln_smp_id_);

		ar & YGGR_SERIALIZATION_NVP(sample_vcxproj_);
		ar & YGGR_SERIALIZATION_NVP(sample_vcxporj_filter_);

		ar & YGGR_SERIALIZATION_NVP(sample_vcxproj_re_);
		ar & YGGR_SERIALIZATION_NVP(sample_vcxproj_fre_);

		ar & YGGR_SERIALIZATION_NVP(dir_filter_);
		ar & YGGR_SERIALIZATION_NVP(file_filter_);
	}

public:
	string_type root_dir_;
	string_type regular_sln_fname_;
	string_type test_bat_fname_;
	
	string_type sample_sln_begin_;
	string_type sample_sln_end_;

	string_type sample_sln_proj_begin_;
	string_type sample_sln_proj_end_;

	string_type sample_sln_smp_dir_;
	string_type sample_sln_smp_proj_;
	string_type sample_sln_smp_id_;

	string_type sample_vcxproj_;
	string_type sample_vcxporj_filter_;

	string_type sample_vcxproj_re_;
	string_type sample_vcxproj_fre_;

	filter_list_type dir_filter_;
	filter_list_type file_filter_;
};

namespace swap_support
{
	YGGR_PP_SWAP_THIS_REF_DEF(test_project_maker_cfg)
} // namespace swap_support

namespace std
{

using swap_support::swap;

} // namespace std

namespace boost
{

using swap_support::swap;

} // namespace boost

#endif // __TEST_PROJECT_MAKER_CFG_HPP__
