// tool_upgrade_vcxproj_cfg.hpp

#ifndef __TOOL_UPGRADE_VCXPROJ_CFG_HPP__
#define __TOOL_UPGRADE_VCXPROJ_CFG_HPP__

#include <yggr/base/yggrdef.h>

#if !(BOOST_VERSION < 105600)

#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/serialization/access.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/container/unordered_set.hpp>

#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/unordered_set.hpp>


struct tool_upgrade_vcxproj_cfg_sample {};

struct tool_upgrade_vcxproj_cfg
{
public:
	typedef yggr::utf8_string string_type;
	typedef yggr::unordered_set<string_type> filter_list_type;

private:
	typedef tool_upgrade_vcxproj_cfg this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type);

public:
	tool_upgrade_vcxproj_cfg(void);
	tool_upgrade_vcxproj_cfg(tool_upgrade_vcxproj_cfg_sample);

	tool_upgrade_vcxproj_cfg(BOOST_RV_REF(this_type) right)
		: root_dir_(boost::move(right.root_dir_)),
			dir_filter_(boost::move(right.dir_filter_)),
			file_filter_(boost::move(right.file_filter_))
	{
	}

	tool_upgrade_vcxproj_cfg(const this_type& right);
	~tool_upgrade_vcxproj_cfg(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;

		if(this == &right_ref)
		{
			return *this;
		}

		yggr::copy_or_move_or_swap(root_dir_, boost::move(right_ref.root_dir_));
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

		ar & YGGR_SERIALIZATION_NVP(dir_filter_);
		ar & YGGR_SERIALIZATION_NVP(file_filter_);
	}

public:
	string_type root_dir_;

	filter_list_type dir_filter_;
	filter_list_type file_filter_;
};

namespace swap_support
{
	YGGR_PP_SWAP_THIS_REF_DEF(tool_upgrade_vcxproj_cfg)
} // namespace swap_support

namespace std
{

using swap_support::swap;

} // namespace std

namespace boost
{

using swap_support::swap;

} // namespace boost

#endif // #if !(BOOST_VERSION < 105600)

#endif // __TOOL_UPGRADE_VCXPROJ_CFG_HPP__
