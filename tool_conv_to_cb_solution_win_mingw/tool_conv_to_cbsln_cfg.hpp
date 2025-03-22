// tool_conv_to_cbsln_cfg.hpp

#ifndef __TOOL_CONV_TO_CBSLN_CFG_HPP__
#define __TOOL_CONV_TO_CBSLN_CFG_HPP__

#include <yggr/base/yggrdef.h>

#if !(BOOST_VERSION < 105600)

#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/serialization/access.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/container/unordered_set.hpp>

#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/unordered_set.hpp>


struct tool_conv_to_cbsln_cfg_sample {};

struct tool_conv_to_cb_solution_cfg
{
public:
	typedef yggr::utf8_string string_type;
	typedef yggr::unordered_set<string_type> filter_list_type;

private:
	typedef tool_conv_to_cb_solution_cfg this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type);

public:
	tool_conv_to_cb_solution_cfg(void);
	tool_conv_to_cb_solution_cfg(tool_conv_to_cbsln_cfg_sample);

	tool_conv_to_cb_solution_cfg(BOOST_RV_REF(this_type) right)
		: cbp_sample_cfg_(boost::move(right.cbp_sample_cfg_)),
			lib_depend_info_cfg_(boost::move(right.lib_depend_info_cfg_)),
			root_dir_(boost::move(right.root_dir_)),
			dir_filter_(boost::move(right.dir_filter_)),
			file_filter_(boost::move(right.file_filter_))
	{
	}

	tool_conv_to_cb_solution_cfg(const this_type& right);
	~tool_conv_to_cb_solution_cfg(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;

		if(this == &right_ref)
		{
			return *this;
		}

		yggr::copy_or_move_or_swap(cbp_sample_cfg_, boost::move(right_ref.cbp_sample_cfg_));
		yggr::copy_or_move_or_swap(lib_depend_info_cfg_, boost::move(right_ref.lib_depend_info_cfg_));

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
		ar & YGGR_SERIALIZATION_NVP(cbp_sample_cfg_);
		ar & YGGR_SERIALIZATION_NVP(lib_depend_info_cfg_);

		ar & YGGR_SERIALIZATION_NVP(root_dir_);

		ar & YGGR_SERIALIZATION_NVP(dir_filter_);
		ar & YGGR_SERIALIZATION_NVP(file_filter_);
	}

public:
	string_type cbp_sample_cfg_;
	string_type lib_depend_info_cfg_;

	string_type root_dir_;

	filter_list_type dir_filter_;
	filter_list_type file_filter_;
};

namespace swap_support
{
	YGGR_PP_SWAP_THIS_REF_DEF(tool_conv_to_cb_solution_cfg)
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

#endif // __TOOL_CONV_TO_CBSLN_CFG_HPP__
