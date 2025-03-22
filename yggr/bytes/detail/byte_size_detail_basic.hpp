//byte_size_detail_basic.hpp

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

#ifndef __YGGR_BYTES_BYTE_SIZE_DETAIL_BASIC_HPP__
#define __YGGR_BYTES_BYTE_SIZE_DETAIL_BASIC_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/has_member_check.hpp>
#include <yggr/mplex/strict_sizeof.hpp>

#include <yggr/iterator_ex/is_iterator.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/size_t.hpp>

#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/is_class.hpp>

#include <boost/preprocessor/arithmetic/sub.hpp>

#include <utility>
#include <numeric>
#include <complex>
#include <iterator>

//#include <yggr/container/unwrap_container.hpp>

namespace yggr
{
namespace bytes
{
namespace detail
{

YGGR_PP_HAS_MEMBER_FOO_DEF(mem_byte_size, T, u64, byte_size, YGGR_PP_SYMBOL_BOOL_TRUE(), void) // 1
YGGR_PP_HAS_STATIC_MEMBER_FOO_DEF(mem_byte_size, u64, byte_size, void) // 2
YGGR_PP_HAS_STATIC_MEMBER_FOO_DEF(mem_byte_size_self, u64, byte_size, const Container&) // 4

template<typename T>
struct get_has_foo_id
	: public
		boost::mpl::if_
		<
			has_mem_byte_size<T>,
			boost::mpl::size_t<1>,
			typename
				boost::mpl::if_
				<
					boost::mpl::or_
					<
						has_static_mem_byte_size<T>,
						has_static_mem_byte_size_self<T>
					>,
					boost::mpl::size_t
					<
						(static_cast<std::size_t>(has_static_mem_byte_size<T>::value) << 1)
						| (static_cast<std::size_t>(has_static_mem_byte_size_self<T>::value) << 2)
					>,
					boost::mpl::size_t<static_cast<std::size_t>(0)>
				>::type
		>::type
{
};

struct acc_op
{
public:
	template<typename T, typename H> inline
	static u64 foo(u64 now, const T& val, const H& h)
	{
		return now + h(val);
	}
};

template<typename T, std::size_t has_byte_size_foo>
struct byte_size_helper_default;

// non-has
template<typename T>
struct byte_size_helper_default<T, static_cast<std::size_t>(0)>
	: public boost::mpl::false_
{
public:
	typedef T value_type;

	inline u64 operator()(const value_type&) const
	{
		return mplex::compatible_strict_sizeof<value_type>::value;
	}

	inline u64 operator()(void) const
	{
		return mplex::compatible_strict_sizeof<value_type>::value;
	}
};

// has-member only
template<typename T>
struct byte_size_helper_default<T, static_cast<std::size_t>(1)>
	: public boost::mpl::true_
{
public:
	typedef T value_type;

	inline u64 operator()(const value_type& val) const
	{
		return val.byte_size();
	}
};

// has-static only
template<typename T>
struct byte_size_helper_default<T, static_cast<std::size_t>(2)>
	: public boost::mpl::false_
{
public:
	typedef T value_type;

	inline u64 operator()(const value_type&) const
	{
		return value_type::byte_size();
	}

	inline u64 operator()(void) const
	{
		return value_type::byte_size();
	}
};

// has-static_self only
template<typename T>
struct byte_size_helper_default<T, static_cast<std::size_t>(4)>
	: public boost::mpl::true_
{
public:
	typedef T value_type;

	inline u64 operator()(const value_type& val) const
	{
		return value_type::byte_size(val);
	}
};

/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! although the two static "byte_size" functions are supported at the same time,			!!!
!!!	if the number of bytes returned by the two functions is different,						!!!
!!!		if the "byte_size" function is called,												!!!
!!!			if you encounter can not be recursive calculation of the case (e.g std::array),	!!!
!!!			will call the "static u64 you_type::byte_size(void)" function,					!!!
!!!			oher case call the "static u64 you_type::byte_size(const you_type&)" function,	!!!
!!!	so you have to know what you need to do													!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

// has-static and static_self only
template<typename T>
struct byte_size_helper_default<T, static_cast<std::size_t>(6)>
	: public boost::mpl::false_
{
public:
	typedef T value_type;

	inline u64 operator()(const value_type& val) const
	{
		return value_type::byte_size(val);
	}

	inline u64 operator()(void) const
	{
		return value_type::byte_size();
	}
};

/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!	byte_size_helper inherit rule:							!!!
!!! "true_": need recursive operation,						!!!
!!!		implement:											!!!
!!!		"inline u64 operator()(const value_type&) const"	!!!
!!!		function											!!!
!!!															!!!
!!! "false_": can't need recursive operation,				!!!
!!!		implement:											!!!
!!!		"inline u64 operator()(const value_type&) const"	!!!
!!!		and													!!!
!!!		"inline u64 operator()(void) const"					!!!
!!!		function											!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

template<typename T>
struct byte_size_helper
	: public byte_size_helper_default<T, get_has_foo_id<T>::value>
{
public:
	typedef T value_type;

protected:
	typedef byte_size_helper_default<T, get_has_foo_id<T>::value> base_type;

public:
	inline u64 operator()(const value_type& val) const
	{
		return base_type::operator()(val);
	}

	inline u64 operator()(void)const
	{
		return base_type::operator()();
	}
};

template<typename T> inline
u64 byte_size(const T& val)
{
	typedef T cv_type;
	typedef typename boost::remove_cv<cv_type>::type type;
	typedef bytes::detail::byte_size_helper<type> h_type;

	h_type h;
	return h(val);
}

template<typename Iter, bool is_class>
struct byte_size_iterator_helper;

template<typename Iter>
struct byte_size_iterator_helper<Iter, true>
{
	typedef Iter iter_type;

	inline u64 operator()(iter_type s, iter_type e) const
	{
		typedef typename boost::iterator_value<iter_type>::type cv_sub_val_type;
		typedef typename boost::remove_cv<cv_sub_val_type>::type sub_val_type;
		typedef bytes::detail::byte_size_helper<sub_val_type> h_type;

		h_type h;
		return std::accumulate(
				s, e, 0, boost::bind(&acc_op::foo<sub_val_type, h_type>,
										 _1, _2, boost::cref(h)));
	}
};

template<typename Iter>
struct byte_size_iterator_helper<Iter, false>
{
	typedef Iter iter_type;

	inline u64 operator()(iter_type s, iter_type e) const
	{
		typedef typename boost::iterator_value<iter_type>::type cv_sub_val_type;
		typedef typename boost::remove_cv<cv_sub_val_type>::type sub_val_type;
		typedef bytes::detail::byte_size_helper<sub_val_type> h_type;

		h_type h;
		return std::abs(std::distance(s, e)) * h();
	}
};

template<typename Iter> inline
typename boost::enable_if<is_iterator<Iter>, u64>::type
	byte_size(Iter s, Iter e)
{
	typedef Iter iter_type;
	typedef typename boost::iterator_value<iter_type>::type cv_sub_val_type;
	typedef typename boost::remove_cv<cv_sub_val_type>::type sub_val_type;
	typedef boost::is_class<sub_val_type> is_class_type;
	typedef byte_size_iterator_helper<iter_type, is_class_type::value> h_type;

	h_type h;
	return h(s, e);
}

} // namespace detail
} // namespace bytes
} // namespace yggr

#endif // __YGGR_BYTES_BYTE_SIZE_DETAIL_BASIC_HPP__
