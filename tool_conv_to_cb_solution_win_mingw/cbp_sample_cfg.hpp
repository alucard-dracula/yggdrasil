// cbp_sample_cfg.hpp

#ifndef __CBP_SAMPLE_CFG_HPP__
#define __CBP_SAMPLE_CFG_HPP__

#include <yggr/base/yggrdef.h>

#if !(BOOST_VERSION < 105600)

#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/serialization/access.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/container/unordered_map.hpp>

#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/unordered_map.hpp>


struct cbp_sample_cfg_sample {};

struct cbp_sample_cfg
{
public:
	typedef yggr::string string_type;
	typedef yggr::unordered_map<string_type, yggr::utf8_string> sample_file_map_type;

private:
	typedef cbp_sample_cfg this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type);

public:
	cbp_sample_cfg(void);
	cbp_sample_cfg(cbp_sample_cfg_sample);

	cbp_sample_cfg(BOOST_RV_REF(this_type) right)
		: file_tag_(boost::move(right.file_tag_)),
			sample_file_map_(boost::move(right.sample_file_map_))
	{
	}

	cbp_sample_cfg(const this_type& right);
	~cbp_sample_cfg(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;

		if(this == &right_ref)
		{
			return *this;
		}

		yggr::copy_or_move_or_swap(file_tag_, boost::move(right_ref.file_tag_));
		yggr::copy_or_move_or_swap(sample_file_map_, boost::move(right_ref.sample_file_map_));

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
		ar & YGGR_SERIALIZATION_NVP(file_tag_);
		ar & YGGR_SERIALIZATION_NVP(sample_file_map_);
	}

public:
	yggr::utf8_string file_tag_;
	sample_file_map_type sample_file_map_;
};

namespace swap_support
{
	YGGR_PP_SWAP_THIS_REF_DEF(cbp_sample_cfg)
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
