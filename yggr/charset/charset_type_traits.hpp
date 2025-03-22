//charset_type_traits.hpp

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

#ifndef __YGGR_CHARSET_CHARSET_TYPE_TRAITS_HPP__
#define __YGGR_CHARSET_CHARSET_TYPE_TRAITS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/compatibility/boost_iterator.hpp>

#include <yggr/type_traits/is_same_tpl.hpp>
#include <yggr/type_traits/native_t.hpp>
#include <yggr/container_ex/is_container.hpp>
#include <yggr/iterator_ex/is_const_iterator.hpp>
#include <yggr/iterator_ex/is_reverse_iterator.hpp>

#include <yggr/charset/string_decl.hpp>

#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/iterator/reverse_iterator.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/or.hpp>

#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_same.hpp>

#include <iterator>

namespace yggr
{
namespace charset
{

// is_utf8_char_t
template<typename T>
struct is_utf8_char_t
	: public boost::mpl::false_
{
};

template<typename T>
struct is_not_utf8_char_t
	: public boost::mpl::not_ < is_utf8_char_t<T> >::type
{
};

// is_utf8_char_reference_t

template<typename T>
struct is_utf8_char_reference_t
	: public boost::mpl::false_
{
};

template<typename T>
struct is_not_utf8_char_reference_t
	: public boost::mpl::not_ < is_utf8_char_reference_t<T> >::type
{
};

// is_utf8_char_const_reference_t

template<typename T>
struct is_utf8_char_const_reference_t
	: public boost::mpl::false_
{
};

template<typename T>
struct is_not_utf8_char_const_reference_t
	: public boost::mpl::not_ < is_utf8_char_reference_t<T> >::type
{
};

// is_utf8_char_view_t

template<typename T>
struct is_utf8_char_view_t
	: public boost::mpl::false_
{
};

template<typename T>
struct is_not_utf8_char_view_t
	: public boost::mpl::not_ < is_utf8_char_view_t<T> >::type
{
};

// is_basic_string_t
template<typename T>
struct is_basic_string_t
	: public boost::mpl::false_
{
};

template<typename T>
struct is_not_basic_string_t
	: public boost::mpl::not_< is_basic_string_t<T> >::type
{
};

// is_basic_string_view_t
template<typename T>
struct is_basic_string_view_t
	: public boost::mpl::false_
{
};

template<typename T>
struct is_not_basic_string_view_t
	: public boost::mpl::not_< is_basic_string_view_t<T> >::type
{
};

template<typename T1, typename T2>
struct is_same_basic_string_view_tpl
	: public
		boost::mpl::and_
		<
			is_basic_string_view_t<T1>,
			is_basic_string_view_t<T2>,
			is_same_tpl<T1, T2>
		>::type
{
};

template<typename T1, typename T2>
struct is_difference_basic_string_view_tpl
	: public
		boost::mpl::and_
		<
			is_basic_string_view_t<T1>,
			is_basic_string_view_t<T2>,
			boost::mpl::not_< is_same_tpl<T1, T2> >
		>::type
{
};

// is_utf8_string_t
template<typename T>
struct is_utf8_string_t
	: public boost::mpl::false_
{
};

template<typename T>
struct is_not_utf8_string_t
	: public boost::mpl::not_ < is_utf8_string_t<T> >::type
{
};

template<typename T>
struct is_utf8_string_view_t
	: public boost::mpl::false_
{
};

template<typename T>
struct is_not_utf8_string_view_t
	: public boost::mpl::not_ < is_utf8_string_view_t<T> >::type
{
};


// is_string_t
template<typename T>
struct is_string_t
	: public
		boost::mpl::or_
		<
			is_basic_string_t<T>,
			is_utf8_string_t<T>
		>::type
{
};

template<typename T>
struct is_not_string_t
	: public boost::mpl::not_< is_string_t<T> >::type
{
};

template<typename T>
struct is_string_view_t
	: public
		boost::mpl::or_
		<
			is_basic_string_view_t<T>,
			is_utf8_string_view_t<T>
		>
{
};

template<typename T>
struct is_not_string_view_t
	: public boost::mpl::not_< is_string_view_t<T> >::type
{
};

// is_generalized_string_t
namespace detail
{

template<typename T,
			bool is_container = container_ex::is_container<T>::value,
			bool is_pointer =
				(boost::is_pointer<T>::value
					&& !boost::is_same<void, typename native_t<T>::type>::value
					&& !boost::is_same<const void, typename native_t<T>::type>::value) >
struct is_generalized_string_t_helper
	: public boost::mpl::false_
{
};


template<typename T>
struct is_generalized_string_t_helper<T, true, false>
	: public
		boost::mpl::bool_
		<
			boost::is_integral< typename boost::range_value<T>::type >::value
		>
{
};

template<typename T>
struct is_generalized_string_t_helper<T, false, true>
	: public
		boost::mpl::bool_
		<
			boost::is_integral< typename boost::iterator_value<T>::type >::value
		>
{
};


} // namespace detail

template<typename T>
struct is_generalized_string_t
	: public
		boost::mpl::bool_
		<
			boost::mpl::or_
			<
				is_string_t<T>,
				is_string_view_t<T>,
				detail::is_generalized_string_t_helper<T>
			>::type::value
		>
{
};

template<typename T>
struct is_not_generalized_string_t
	: public boost::mpl::not_< is_generalized_string_t<T> >::type
{
};


template<typename T>
struct is_convertible_utf8_string
	: public is_generalized_string_t<T>
{
};

// iterators

// is_utf8_string_iterator_t

template<typename T>
struct is_utf8_string_iterator_t
	: public boost::mpl::false_
{
};

template<typename T>
struct is_not_utf8_string_iterator_t
	: public boost::mpl::not_ < is_utf8_string_iterator_t<T> >::type
{
};

// riter
namespace detail
{

template<typename T, typename IsRiter>
struct is_utf8_string_reverse_iterator_t_helper
	: public boost::mpl::false_
{
};

template<typename T>
struct is_utf8_string_reverse_iterator_t_helper<T, boost::mpl::true_>
	: public is_utf8_string_iterator_t<typename T::iterator_type>
{
};

} // namespace detail

template<typename T>
struct is_utf8_string_reverse_iterator_t
	: public
		detail::is_utf8_string_reverse_iterator_t_helper
		<
			T,
			is_reverse_iterator<T>
		>::type
{
};

template<typename T>
struct is_not_utf8_string_reverse_iterator_t
	: public boost::mpl::not_ < is_utf8_string_reverse_iterator_t<T> >::type
{
};

// const_iterator
template<typename T>
struct is_utf8_string_const_iterator_t
	: public boost::mpl::false_
{
};

template<typename T>
struct is_not_utf8_string_const_iterator_t
	: public boost::mpl::not_ < is_utf8_string_const_iterator_t<T> >::type
{
};

// criter
namespace detail
{

template<typename T, typename IsRiter>
struct is_utf8_string_const_reverse_iterator_t_helper
	: public boost::mpl::false_
{
};

template<typename T>
struct is_utf8_string_const_reverse_iterator_t_helper<T, boost::mpl::true_>
	: public is_utf8_string_const_iterator_t<typename T::iterator_type>
{
};

} // namespace detail

template<typename T>
struct is_utf8_string_const_reverse_iterator_t
	: public
		detail::is_utf8_string_const_reverse_iterator_t_helper
		<
			T,
			is_reverse_iterator<T>
		>::type
{
};

template<typename T>
struct is_not_utf8_string_const_reverse_iterator_t
	: public boost::mpl::not_ < is_utf8_string_const_reverse_iterator_t<T> >::type
{
};

// modify_iterator
template<typename T>
struct is_utf8_string_modify_iterator_t
	: public boost::mpl::false_
{
};

template<typename T>
struct is_not_utf8_string_modift_iterator_t
	: public boost::mpl::not_ < is_utf8_string_modify_iterator_t<T> >::type
{
};

// rmiter
namespace detail
{

template<typename T, typename IsRiter>
struct is_utf8_string_modify_reverse_iterator_t_helper
	: public boost::mpl::false_
{
};

template<typename T>
struct is_utf8_string_modify_reverse_iterator_t_helper<T, boost::mpl::true_>
	: public is_utf8_string_modify_iterator_t<typename T::iterator_type>
{
};

} // namespace detail

template<typename T>
struct is_utf8_string_modify_reverse_iterator_t
	: public
		detail::is_utf8_string_modify_reverse_iterator_t_helper
		<
			T,
			is_reverse_iterator<T>
		>::type
{
};

template<typename T>
struct is_not_utf8_string_modify_reverse_iterator_t
	: public boost::mpl::not_ < is_utf8_string_modify_reverse_iterator_t<T> >::type
{
};

// any iter
template<typename T>
struct is_any_utf8_string_iterator_t
	: public
		boost::mpl::or_
		<
			boost::mpl::or_
			<
				is_utf8_string_iterator_t<T>,
				is_utf8_string_reverse_iterator_t<T>
			>,
			boost::mpl::or_
			<
				is_utf8_string_const_iterator_t<T>,
				is_utf8_string_const_reverse_iterator_t<T>
			>,
			boost::mpl::or_
			<
				is_utf8_string_modify_iterator_t<T>,
				is_utf8_string_modify_reverse_iterator_t<T>
			>
		>::type
{
};

template<typename T>
struct is_not_any_utf8_string_iterator_t
	: public boost::mpl::not_ < is_any_utf8_string_iterator_t<T> >::type
{
};

// any reverse_iter
template<typename T>
struct is_any_utf8_string_reverse_iterator_t
	: public
		boost::mpl::or_
		<
			is_utf8_string_reverse_iterator_t<T>,
			is_utf8_string_const_reverse_iterator_t<T>,
			is_utf8_string_modify_reverse_iterator_t<T>
		>::type
{
};

template<typename T>
struct is_not_any_utf8_string_reverse_iterator_t
	: public boost::mpl::not_ < is_any_utf8_string_reverse_iterator_t<T> >::type
{
};

} // namespace charset
} // namespace yggr


// is_basic_string_t
namespace yggr
{
namespace charset
{

template<typename Char, typename Traits, typename Alloc>
struct is_basic_string_t< std::basic_string<Char, Traits, Alloc> >
	: public boost::mpl::true_
{
};

template<typename Char, typename Traits, typename Alloc>
struct is_basic_string_t< boost::container::basic_string<Char, Traits, Alloc> >
	: public boost::mpl::true_
{
};

} // namespace charset
} // namespace yggr

// is_basic_string_view_t

#ifndef YGGR_NO_CXX17_HDR_STRING_VIEW

#include <string_view>

namespace yggr
{
namespace charset
{

template<typename Char, typename Traits>
struct is_basic_string_view_t< std::basic_string_view<Char, Traits> >
	: public boost::mpl::true_
{
};

} // namespace charset
} // namespace yggr

#endif //YGGR_NO_CXX17_HDR_STRING_VIEW

#endif // __YGGR_CHARSET_CHARSET_TYPE_TRAITS_HPP__
