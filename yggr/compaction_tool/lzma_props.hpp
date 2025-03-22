//lzma_props.hpp

/****************************************************************************
Copyright (c) 2010-2024 yggdrasil

author: xu yang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __YGGR_COMPACTION_TOOL_LZMA_PROPS_HPP__
#define __YGGR_COMPACTION_TOOL_LZMA_PROPS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/container/array.hpp>
#include <yggr/move/move.hpp>
#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/array.hpp>

#include <boost/ref.hpp>

#include <cassert>
#include <memory>


namespace yggr
{
namespace compaction_tool
{

struct lzma_props
	: public ::yggr::array<yggr::u8, 5>
{
public:
	YGGR_STATIC_CONSTANT(yggr::u32, E_length = 5);
	YGGR_STATIC_CONSTANT(yggr::u32, E_byte_length = 5);

	typedef yggr::u8 val_type;
	typedef ::yggr::array<val_type, E_length> base_type;
	typedef base_type::value_type value_type;

private:
	typedef lzma_props this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	lzma_props(void);

	template<typename InputIter>
	lzma_props(InputIter s, InputIter e)
		: base_type(s, e)
	{
	}

	lzma_props(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	lzma_props(const this_type& right);

	~lzma_props(void);

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		base_type& base = *this;
		copy_or_move_or_swap(base, boost::move(static_cast<base_type&>(right)));

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

public:
	inline bool compare_eq(const this_type& right) const
	{
		const base_type& l = *this;
		const base_type& r = *this;
		return
			(boost::addressof(l) == boost::addressof(r))
			|| (0 == memcmp(&l[0], &r[0], E_byte_length));
	}

public:
	inline static std::size_t prop_size(void)
	{
		return static_cast<std::size_t>(this_type::E_length);
	}

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP("data", boost::serialization::base_object<base_type>(*this));
	}
};

// non-member function

inline bool operator==(const lzma_props& l, const lzma_props& r)
{
	return l.compare_eq(r);
}

inline bool operator!=(const lzma_props& l, const lzma_props& r)
{
	return !l.compare_eq(r);
}


} // compaction_tool
} // namespace yggr

namespace yggr
{
namespace compaction_tool
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(lzma_props)
} // namespace swap_support

using swap_support::swap; 

} // namespace compaction_tool
} // namespace yggr

namespace std
{
	using ::yggr::compaction_tool::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::compaction_tool::swap_support::swap;
} // namespace boost

#endif // __YGGR_COMPACTION_TOOL_LZMA_PROPS_HPP__
