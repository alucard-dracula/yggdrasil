// utf8_string_or_string_view_t_getter.hpp

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

#ifndef __YGGR_CHARSET_UTF8_STRING_OR_STRING_VIEW_T_GETTER_HPP__
#define __YGGR_CHARSET_UTF8_STRING_OR_STRING_VIEW_T_GETTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/get_reference_if_in.hpp>
#include <yggr/charset/charset_type_traits.hpp>

#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/mpl/vector.hpp>

namespace yggr
{
namespace charset
{
namespace detail
{

template<typename String, typename Utf8String, typename Utf8StringView>
struct utf8_string_or_string_view_t_getter
	: public
		mplex::get_reference_if_in
		<
			String, 
			Utf8String,
			boost::mpl::vector<Utf8String, Utf8StringView>
		>
{
protected:
	typedef 
		mplex::get_reference_if_in
		<
			String, 
			Utf8String,
			boost::mpl::vector<Utf8String, Utf8StringView>
		> base_type;
public:
	typedef typename base_type::nctype nctype;
	typedef typename base_type::ctype ctype;
private:
	BOOST_MPL_ASSERT((charset::is_utf8_string_t<Utf8String>));
	BOOST_MPL_ASSERT((charset::is_utf8_string_view_t<Utf8StringView>));
};

} // namespace detail
} // namespace charset
} // namespace yggr

//YGGR_TYPEDEF_UTF8_STRING_OR_STRING_VIEW_TYPE
#define YGGR_TYPEDEF_UTF8_STRING_OR_STRING_VIEW_TYPE(__string_t__, __utf8_sring_or_string_view_t_name__ ) \
	typedef yggr::charset::detail::utf8_string_or_string_view_t_getter< \
				__string_t__, \
				yggr::charset::utf8_string, \
				yggr::charset::utf8_string_view >::nctype __utf8_sring_or_string_view_t_name__

#define YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE(__string_t__, __utf8_sring_or_string_view_t_name__) \
	typedef yggr::charset::detail::utf8_string_or_string_view_t_getter< \
				__string_t__, \
				yggr::charset::utf8_string, \
				yggr::charset::utf8_string_view >::ctype __utf8_sring_or_string_view_t_name__

#define YGGR_TYPEDEF_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(__string_t__, __utf8_sring_or_string_view_t_name__) \
	typedef typename yggr::charset::detail::utf8_string_or_string_view_t_getter< \
						__string_t__, \
						yggr::charset::utf8_string, \
						yggr::charset::utf8_string_view >::nctype __utf8_sring_or_string_view_t_name__

#define YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(__string_t__, __utf8_sring_or_string_view_t_name__) \
	typedef typename yggr::charset::detail::utf8_string_or_string_view_t_getter< \
						__string_t__, \
						yggr::charset::utf8_string, \
						yggr::charset::utf8_string_view >::ctype __utf8_sring_or_string_view_t_name__

//YGGR_TYPEDEF_CUSTOM_UTF8_STRING_OR_STRING_VIEW_TYPE
#define YGGR_TYPEDEF_CUSTOM_UTF8_STRING_OR_STRING_VIEW_TYPE(__string_t__, \
															__cmp_utf8_string_t__, \
															__cmp_utf8_string_view_t__, \
															__utf8_sring_or_string_view_t_name__) \
	typedef yggr::charset::detail::utf8_string_or_string_view_t_getter< \
				__string_t__, \
				__cmp_utf8_string_t__, \
				__cmp_utf8_string_view_t__ >::nctype __utf8_sring_t_name__

#define YGGR_TYPEDEF_CUSTOM_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE(__string_t__, \
																	__cmp_utf8_string_t__, \
																	__cmp_utf8_string_view_t__, \
																	__utf8_sring_or_string_view_t_name__) \
	typedef yggr::charset::detail::utf8_string_or_string_view_t_getter< \
				__string_t__, \
				__cmp_utf8_string_t__, \
				__cmp_utf8_string_view_t__ >::ctype __utf8_sring_t_name__

#define YGGR_TYPEDEF_CUSTOM_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(__string_t__, \
																__cmp_utf8_string_t__, \
																__cmp_utf8_string_view_t__, \
																__utf8_sring_or_string_view_t_name__) \
	typedef typename yggr::charset::detail::utf8_string_or_string_view_t_getter< \
						__string_t__, \
						__cmp_utf8_string_t__, \
						__cmp_utf8_string_view_t__ >::nctype __utf8_sring_t_name__

#define YGGR_TYPEDEF_CUSTOM_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(__string_t__, \
																	__cmp_utf8_string_t__, \
																	__cmp_utf8_string_view_t__, \
																	__utf8_sring_or_string_view_t_name__) \
	typedef typename yggr::charset::detail::utf8_string_or_string_view_t_getter< \
						__string_t__, \
						__cmp_utf8_string_t__, \
						__cmp_utf8_string_view_t__  >::ctype __utf8_sring_t_name__

#endif // __YGGR_CHARSET_UTF8_STRING_OR_STRING_VIEW_T_GETTER_HPP__