//convertible_traits.hpp

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

#ifndef __YGGR_CHARSET_CONVERTIBLE_TRAITS_HPP__
#define __YGGR_CHARSET_CONVERTIBLE_TRAITS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/is_enum.hpp>
#include <yggr/type_traits/number_t.hpp>

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_pointer.hpp>

#include <boost/mpl/and.hpp>

namespace yggr
{
namespace charset
{

namespace detail
{

template<typename T>
struct patch_is_integral_c8
{
	typedef T type;
};

#if (BOOST_VERSION < 108200) && !defined(YGGR_NO_CHAR8_T)

template<>
struct patch_is_integral_c8< ::yggr::c8 >
{
	typedef char type;
};

template<>
struct patch_is_integral_c8< const ::yggr::c8 >
{
	typedef const char type;
};


#endif // (BOOST_VERSION < 108200) && !defined(YGGR_NO_CHAR8_T)

} //namespace detail

// is_can_be_conv_into_char
template<typename T>
struct is_can_be_conv_into_char
	: public
		boost::mpl::or_
		<
			boost::is_arithmetic
			<
				typename
					detail::patch_is_integral_c8
					<
						typename
							boost::remove_cv
							<
								typename boost::remove_reference<T>::type
							>::type
					>::type
				
			>,
			is_enum
			<
				typename
					boost::remove_cv
					<
						typename boost::remove_reference<T>::type
					>::type
			>
		>::type
{
};
	
// is_convertible_char
template<typename T>
struct is_convertible_char
	: public
		boost::mpl::bool_
		<
			boost::is_integral
			<
				typename detail::patch_is_integral_c8<T>::type
			>::value
		>::type
{
};

template<typename T>
struct format_char_t
	: public YGGR_PP_INTEGER_T((sizeof(T) <= sizeof(s32)? sizeof(T) : sizeof(s32)))
{
};

} // namespace charset
} // namespace yggr

#define YGGR_PP_FORMAT_CHAR_TYPE( __type__ ) ::yggr::charset::format_char_t< __type__ >::type

#define YGGR_PP_FORMAT_CHAR_TYPE_TPL( __type__ ) typename YGGR_PP_FORMAT_CHAR_TYPE( __type__ )

#endif // __YGGR_CHARSET_CONVERTIBLE_TRAITS_HPP__