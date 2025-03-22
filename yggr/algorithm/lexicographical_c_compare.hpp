// lexicographical_c_compare.hpp

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

#ifndef __YGGR_ALGORITHM_LEXICOGRAPHICAL_C_COMPARE_HPP__
#define __YGGR_ALGORITHM_LEXICOGRAPHICAL_C_COMPARE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bytes/byte_size_sp_basic.hpp>
#include <yggr/type_traits/upper_types.hpp>

#include <yggr/iterator_ex/is_reverse_iterator.hpp>
#include <yggr/iterator_ex/iterator_category_check.hpp>
#include <yggr/iterator_ex/iterator_traits.hpp>
#include <yggr/math/sign.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_unsigned.hpp>

#include <boost/mpl/and.hpp>

#include <memory>
#include <algorithm>


namespace yggr
{
namespace algorithm
{
namespace detail
{

struct c_cmp
{
	template<typename T1, typename T2> inline
	s32 operator()(const T1& l, const T2& r) const
	{
		return l == r? 0 : l < r? -1 : 1;
	}
};

struct c_hex_cmp
{
	template<typename T1, typename T2> inline
	s32 operator()(const T1& l, const T2& r) const
	{
		typedef typename upper_unsigned<T1>::type ut1_type;
		typedef typename upper_unsigned<T2>::type ut2_type;

		return l == r? 0 : static_cast<ut1_type>(l) < static_cast<ut2_type>(r)? -1 : 1;
	}
};

struct c_mem_cmp
{
};

template<typename Iter1, typename Iter2, typename Cmp = c_cmp>
struct memcmpable
	: boost::mpl::false_
{
};

template<typename Iter1, typename Iter2>
struct memcmpable<Iter1, Iter2, c_cmp>
	: boost::mpl::and_
		<
			boost::mpl::and_
			<
				iterator_ex::is_random_access_iterator<Iter1>,
				boost::mpl::not_< iterator_ex::is_reverse_iterator<Iter1> >,
				boost::is_unsigned<typename iterator_ex::iterator_value_ex<Iter1>::type>
			>,
			boost::mpl::and_
			<
				iterator_ex::is_random_access_iterator<Iter2>,
				boost::mpl::not_< iterator_ex::is_reverse_iterator<Iter2> >,
				boost::is_unsigned<typename iterator_ex::iterator_value_ex<Iter2>::type>
			>
		>
{
};

template<typename Iter1, typename Iter2>
struct memcmpable<Iter1, Iter2, c_hex_cmp>
	: boost::mpl::and_
		<
			boost::mpl::and_
			<
				iterator_ex::is_random_access_iterator<Iter1>,
				boost::mpl::not_< iterator_ex::is_reverse_iterator<Iter1> >
			>,
			boost::mpl::and_
			<
				iterator_ex::is_random_access_iterator<Iter2>,
				boost::mpl::not_< iterator_ex::is_reverse_iterator<Iter2> >
			>
		>
{
};

template<typename Iter1, typename Iter2>
struct memcmpable<Iter1, Iter2, c_mem_cmp>
	: boost::mpl::true_
{
};

template<typename V1, typename V2> inline
s32 d_lexicographical_c_compare_rnd(V1* p1, V2* p2, std::size_t p1_byte_size, std::size_t p2_byte_size)
{
	if(p1)
	{
		assert(p1_byte_size);
		if(p2)
		{
			assert(p2_byte_size);
			// in linux gcc memcmp return value is not formated, so using math sign fora
			yggr::s32 cmp_rst = math::sign(memcmp(p1, p2, (std::min)(p1_byte_size, p2_byte_size)));
			return 
				cmp_rst == 0?
					p1_byte_size == p2_byte_size? 0 : p1_byte_size < p2_byte_size? -1 : 1
				: cmp_rst;
		}
		else
		{
			assert(!p2_byte_size);
			return 1;
		}
	}
	else
	{
		assert(!p1_byte_size);
		if(p2)
		{
			assert(p2_byte_size);
			return -1;
		}
		else
		{
			assert(!p2_byte_size);
			return 0;
		}
	}
}

template<typename Iter> inline
typename 
	boost::enable_if
	<
		boost::is_pointer<Iter>,
		Iter
	>::type
	d_unwrap_iterator(Iter iter)
{
	return iter;
}

template<typename Iter> inline
typename 
	boost::disable_if
	<
		boost::is_pointer<Iter>,
		typename iterator_ex::iterator_pointer_ex<Iter>::type
	>::type
	d_unwrap_iterator(Iter iter)
{
	return iter.operator->();
}

template<typename Iter1, typename Iter2> inline
typename
	boost::enable_if
	<
		memcmpable<Iter1, Iter2>,
		s32
	>::type
	d_lexicographical_c_compare(Iter1 iter1_s, Iter1 iter1_e, Iter2 iter2_s, Iter2 iter2_e)
{
	std::size_t p1_byte_size = bytes_sp::byte_size(iter1_s, iter1_e);
	std::size_t p2_byte_size = bytes_sp::byte_size(iter2_s, iter2_e);

	return 
		d_lexicographical_c_compare_rnd(
			p1_byte_size? d_unwrap_iterator(iter1_s) : 0, // must check p1_byte_size, because if not msvc100 -O2 failed optimization
			p2_byte_size? d_unwrap_iterator(iter2_s) : 0, // must check p2_byte_size, because if not msvc100 -O2 failed optimization
			p1_byte_size, p2_byte_size);
}

template<typename Iter1, typename Iter2, typename Cmp> inline
typename
	boost::enable_if
	<
		memcmpable<Iter1, Iter2, Cmp>,
		s32
	>::type
	d_lexicographical_c_compare(Iter1 iter1_s, Iter1 iter1_e, Iter2 iter2_s, Iter2 iter2_e, Cmp cmp)
{
	std::size_t p1_byte_size = bytes_sp::byte_size(iter1_s, iter1_e);
	std::size_t p2_byte_size = bytes_sp::byte_size(iter2_s, iter2_e);

	return 
		d_lexicographical_c_compare_rnd(
			p1_byte_size? d_unwrap_iterator(iter1_s) : 0, // must check p1_byte_size, because if not msvc100 -O2 failed optimization
			p2_byte_size? d_unwrap_iterator(iter2_s) : 0, // must check p2_byte_size, because if not msvc100 -O2 failed optimization
			p1_byte_size, p2_byte_size);
}

template<typename Iter1, typename Iter2, typename Cmp>
typename
	boost::disable_if
	<
		memcmpable<Iter1, Iter2, Cmp>,
		s32
	>::type
	d_lexicographical_c_compare(Iter1 iter1_s, Iter1 iter1_e, Iter2 iter2_s, Iter2 iter2_e, Cmp cmp)
{
	s32 tmp_rst = 0;
	for(;iter1_s != iter1_e && iter2_s != iter2_e && tmp_rst == 0; ++iter1_s, ++iter2_s)
	{
		tmp_rst = cmp(*iter1_s, *iter2_s);
	}

	return 
		tmp_rst == 0? 
			iter1_s == iter1_e? 
				iter2_s == iter2_e? 0 : -1
				: 1
			: tmp_rst;
}

template<typename Iter1, typename Iter2> inline
typename
	boost::disable_if
	<
		memcmpable<Iter1, Iter2>,
		s32
	>::type
	d_lexicographical_c_compare(Iter1 iter1_s, Iter1 iter1_e, Iter2 iter2_s, Iter2 iter2_e)
{
	return d_lexicographical_c_compare(iter1_s, iter1_e, iter2_s, iter2_e, c_cmp());
}

} // namespace detail

using detail::c_cmp;
using detail::c_hex_cmp;
using detail::c_mem_cmp;

template<typename Iter1, typename Iter2> inline
s32 lexicographical_c_compare(Iter1 iter1_s, Iter1 iter1_e, Iter2 iter2_s, Iter2 iter2_e)
{
	return detail::d_lexicographical_c_compare(iter1_s, iter1_e, iter2_s, iter2_e);
}

template<typename Iter1, typename Iter2, typename Cmp> inline
s32 lexicographical_c_compare(Iter1 iter1_s, Iter1 iter1_e, Iter2 iter2_s, Iter2 iter2_e, Cmp cmp)
{
	return detail::d_lexicographical_c_compare(iter1_s, iter1_e, iter2_s, iter2_e, cmp);
}

} // namespace algorithm

using algorithm::c_cmp;
using algorithm::c_hex_cmp;
using algorithm::c_mem_cmp;

using algorithm::lexicographical_c_compare;

} // namespace yggr

#endif // __YGGR_ALGORITHM_LEXICOGRAPHICAL_C_COMPARE_HPP__
