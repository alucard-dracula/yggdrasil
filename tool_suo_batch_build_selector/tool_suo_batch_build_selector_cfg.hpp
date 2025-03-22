// tool_suo_batch_build_selector_cfg.hpp

#ifndef __TOOL_SUO_BATCH_SELECTOR_CFG_HPP__
#define __TOOL_SUO_BATCH_SELECTOR_CFG_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/ref.hpp>

#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/serialization/access.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/container/array.hpp>

#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/array.hpp>


struct tool_suo_batch_build_selector_cfg_sample {};

struct tool_suo_batch_build_selector_cfg
{
public:
	typedef yggr::utf8_string string_type;
	typedef yggr::array<yggr::u8, 4> mark_type;

private:
	typedef tool_suo_batch_build_selector_cfg this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type);

public:
	tool_suo_batch_build_selector_cfg(void);
	tool_suo_batch_build_selector_cfg(tool_suo_batch_build_selector_cfg_sample);

	tool_suo_batch_build_selector_cfg(BOOST_RV_REF(this_type) right)
		: file_suo_(boost::move(right.file_suo_)),
			batch_mark_(boost::move(right.batch_mark_))
	{
	}

	tool_suo_batch_build_selector_cfg(const this_type& right);
	~tool_suo_batch_build_selector_cfg(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;

		if(this == &right_ref)
		{
			return *this;
		}

		yggr::copy_or_move_or_swap(file_suo_, boost::move(right_ref.file_suo_));
		yggr::copy_or_move_or_swap(batch_mark_, boost::move(right_ref.batch_mark_));

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
		ar & YGGR_SERIALIZATION_NVP(file_suo_);
		ar & YGGR_SERIALIZATION_NVP(batch_mark_);
	}

public:
	string_type file_suo_;
	mark_type batch_mark_;
};

namespace swap_support
{
	YGGR_PP_SWAP_THIS_REF_DEF(tool_suo_batch_build_selector_cfg)
} // namespace swap_support

namespace std
{

using swap_support::swap;

} // namespace std

namespace boost
{

using swap_support::swap;

} // namespace boost


#endif // __TOOL_SUO_BATCH_SELECTOR_CFG_HPP__
