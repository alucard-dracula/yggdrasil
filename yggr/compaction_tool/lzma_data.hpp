//lzma_data.hpp

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

#ifndef __YGGR_COMPACTION_TOOL_LZMA_DATA_HPP__
#define __YGGR_COMPACTION_TOOL_LZMA_DATA_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/utility/swap.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/move/move.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/typename_replaceor.hpp>
#include <yggr/charset/string.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/string.hpp>

#include <yggr/compaction_tool/lzma_props.hpp>

#include <boost/type_traits/is_same.hpp>

#include <utility>


namespace yggr
{
namespace compaction_tool
{

template<typename TChar = char,
			typename TTraits = typename charset::basic_string<TChar>::traits_type, 
			typename TAlloc = typename charset::basic_string<TChar>::allocator_type,
			template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String = charset::basic_string
		>
class lzma_data
{
public:
	typedef TChar value_type;
	BOOST_MPL_ASSERT((boost::is_same<value_type, char>));

	typedef ser_size_type size_type;
	typedef TTraits traits_type;
	typedef TAlloc allocator_type;
	typedef lzma_props lzma_props_type;
	typedef Basic_String<value_type, traits_type, allocator_type> lzma_buf_type;

private:
	typedef lzma_data this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	lzma_data(void)
		: _org_byte_size(size_type())
	{
	}

	lzma_data(const size_type& org_byte_size, 
				const lzma_props_type& props, 
				const lzma_buf_type& buf)
		: _org_byte_size(org_byte_size),
			_props(props), _buf(buf)
	{
	}

	lzma_data(const size_type& org_byte_size, 
				const lzma_props_type& props, 
				BOOST_RV_REF(lzma_buf_type) buf)
		: _org_byte_size(org_byte_size),
			_props(props), _buf(boost::move(buf))
	{
	}

	template<typename InputIter>
	lzma_data(const size_type& org_byte_size, 
				const lzma_props_type& props, 
				InputIter s, InputIter e)
		: _org_byte_size(org_byte_size),
			_props(props), _buf(s, e)
	{
	}

	lzma_data(BOOST_RV_REF(this_type) right)
		: _org_byte_size(boost::move(right._org_btye_size)),
			_props(boost::move(right._props)),
			_buf(boost::move(right._buf))
			
	{
	}

	lzma_data(const this_type& right)
		: _org_byte_size(right._org_byte_size),
			_props(right._props),
			_buf(right._buf)
	{
	}

	~lzma_data(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_org_byte_size, boost::move(right._org_byte_size));
		copy_or_move_or_swap(_props, boost::move(right._props));
		copy_or_move_or_swap(_buf, boost::move(right._buf));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) 
		{
			return *this;
		}

		_org_byte_size = right._org_byte_size;
		_props = right._props;
		_buf = right._buf;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		yggr::swap(_org_byte_size, right._org_byte_size);
		_props.swap(right._props);
		_buf.swap(right._buf);
	}

public:
	inline bool empty(void) const
	{
		return !_org_byte_size || _buf.empty();
	}

	inline size_type org_size(void) const
	{
		return _org_byte_size;
	}

	inline const lzma_props_type& props(void) const
	{
		return _props;
	}

	inline const lzma_buf_type& buf(void) const
	{
		return _buf;
	}

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(_org_byte_size);
		ar & YGGR_SERIALIZATION_NVP(_props);
		ar & YGGR_SERIALIZATION_NVP(_buf);
	}

private:
	size_type _org_byte_size;
	lzma_props_type _props;
	lzma_buf_type _buf;
};

} // namespace compaction_tool
} // namespace yggr

namespace yggr
{
namespace compaction_tool
{
namespace swap_support
{

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ),
			template < YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename _T ) > class Basic_String > inline
void swap(YGGR_PP_TEMPLATE_TYPE2(lzma_data,
									YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, T ),
									Basic_String)& l,
			YGGR_PP_TEMPLATE_TYPE2(lzma_data,
									YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, T ),
									Basic_String)& r) 
{
	l.swap(r); 
}


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

#endif // __YGGR_COMPACTION_TOOL_LZMA_DATA_HPP__