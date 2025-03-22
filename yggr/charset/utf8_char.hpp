//utf8_char.hpp

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

#ifndef __YGGR_CHARSET_UTF8_CHAR_HPP__
#define __YGGR_CHARSET_UTF8_CHAR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/get_reference_if_same.hpp>

#include <yggr/charset/utf8_string_impl.hpp>
#include <yggr/charset/utf8_string_view_impl.hpp>
#include <yggr/charset/utf8_string_iterator.hpp>

#include <yggr/charset/utf8_char_impl.hpp>
#include <yggr/charset/utf8_char_view_impl.hpp>
#include <yggr/charset/utf8_char_reference_impl.hpp>

#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>


namespace yggr
{
namespace charset
{

typedef utf8_char_impl<> utf8_char;

namespace detail
{

template<typename Char, typename Utf8_Char>
struct utf8_char_t_getter
	: public mplex::get_reference_if_same<Char, Utf8_Char>
{
private:
	BOOST_MPL_ASSERT((charset::is_utf8_char_t<Utf8_Char>));
};

template<typename T1, typename T2, typename Utf8_Char>
struct utf8_char_t_getter< string_charset_helper<T1, T2>, Utf8_Char >
	: public 
		mplex::get_reference_if_same
		<
			typename 
				boost::remove_cv
				<
					typename boost::remove_reference<T1>::type
				>::type, 
			Utf8_Char
		>
{
private:
	BOOST_MPL_ASSERT((charset::is_utf8_char_t<Utf8_Char>));
};

} // namespace detail

} // namespace charset

using charset::utf8_char;

} // namespace yggr

//YGGR_TYPEDEF_UTF8_CHAR_TYPE
#define YGGR_TYPEDEF_UTF8_CHAR_TYPE(__char_t__, __utf8_char_t_name__) \
	typedef yggr::charset::detail::utf8_char_t_getter< \
				__char_t__, yggr::charset::utf8_char >::nctype __utf8_char_t_name__

#define YGGR_TYPEDEF_CONST_UTF8_CHAR_TYPE(__char_t__, __utf8_char_t_name__) \
	typedef yggr::charset::detail::utf8_char_t_getter< \
				__char_t__, yggr::charset::utf8_char >::ctype __utf8_char_t_name__

#define YGGR_TYPEDEF_UTF8_CHAR_TYPE_TPL(__char_t__, __utf8_char_t_name__) \
	typedef typename yggr::charset::detail::utf8_char_t_getter< \
						__char_t__, yggr::charset::utf8_char >::nctype __utf8_char_t_name__

#define YGGR_TYPEDEF_CONST_UTF8_CHAR_TYPE_TPL(__char_t__, __utf8_char_t_name__) \
	typedef typename yggr::charset::detail::utf8_char_t_getter< \
						__char_t__, yggr::charset::utf8_char >::ctype __utf8_char_t_name__

//YGGR_TYPEDEF_CUSTOM_UTF8_CHAR_TYPE
#define YGGR_TYPEDEF_CUSTOM_UTF8_CHAR_TYPE(__char_t__, __cmp_utf8_char_t__, __utf8_char_t_name__) \
	typedef yggr::charset::detail::utf8_char_t_getter< \
				__char_t__, __cmp_utf8_char_t__ >::nctype __utf8_char_t_name__

#define YGGR_TYPEDEF_CUSTOM_CONST_UTF8_CHAR_TYPE(__char_t__, __cmp_utf8_char_t__, __utf8_char_t_name__) \
	typedef yggr::charset::detail::utf8_char_t_getter< \
				__char_t__, __cmp_utf8_char_t__ >::ctype __utf8_char_t_name__

#define YGGR_TYPEDEF_CUSTOM_UTF8_CHAR_TYPE_TPL(__char_t__, __cmp_utf8_char_t__, __utf8_char_t_name__) \
	typedef typename yggr::charset::detail::utf8_char_t_getter< \
						__char_t__, __cmp_utf8_char_t__ >::nctype __utf8_char_t_name__

#define YGGR_TYPEDEF_CUSTOM_CONST_UTF8_CHAR_TYPE_TPL(__char_t__, __cmp_utf8_char_t__, __utf8_char_t_name__) \
	typedef typename yggr::charset::detail::utf8_char_t_getter< \
						__char_t__, __cmp_utf8_char_t__ >::ctype __utf8_char_t_name__

#endif // __YGGR_CHARSET_UTF8_CHAR_HPP__
