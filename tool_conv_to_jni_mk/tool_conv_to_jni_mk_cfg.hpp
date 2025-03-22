// tool_conv_to_jni_mk_cfg.hpp

#ifndef __TOOL_CONV_TO_JNI_MK_CFG_HPP__
#define __TOOL_CONV_TO_JNI_MK_CFG_HPP__

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


struct tool_conv_to_jni_mk_cfg_sample {};

struct tool_conv_to_jni_mk_cfg
{
public:
	typedef yggr::utf8_string string_type;
	typedef yggr::unordered_set<string_type> filter_list_type;

private:
	typedef tool_conv_to_jni_mk_cfg this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type);

public:
	tool_conv_to_jni_mk_cfg(void);
	tool_conv_to_jni_mk_cfg(tool_conv_to_jni_mk_cfg_sample);

	tool_conv_to_jni_mk_cfg(BOOST_RV_REF(this_type) right)
		: root_dir_(boost::move(right.root_dir_)),
			dir_filter_(boost::move(right.dir_filter_)),
			cbp_file_filter_(boost::move(right.cbp_file_filter_)),
			cpp_file_filter_(boost::move(right.cpp_file_filter_))
	{
	}

	tool_conv_to_jni_mk_cfg(const this_type& right);
	~tool_conv_to_jni_mk_cfg(void);

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
		yggr::copy_or_move_or_swap(cbp_file_filter_, boost::move(right_ref.cbp_file_filter_));
		yggr::copy_or_move_or_swap(cpp_file_filter_, boost::move(right_ref.cpp_file_filter_));

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
		ar & YGGR_SERIALIZATION_NVP(cbp_file_filter_);
		ar & YGGR_SERIALIZATION_NVP(cpp_file_filter_);
	}

public:
	string_type root_dir_;

	filter_list_type dir_filter_;
	filter_list_type cbp_file_filter_;
	filter_list_type cpp_file_filter_;
};

namespace swap_support
{
	YGGR_PP_SWAP_THIS_REF_DEF(tool_conv_to_jni_mk_cfg)
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

#endif // __TOOL_CONV_TO_JNI_MK_CFG_HPP__
