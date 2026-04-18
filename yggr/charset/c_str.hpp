//c_str.hpp

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


#ifndef __YGGR_CHARSET_C_STR_HPP__
#define __YGGR_CHARSET_C_STR_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/mplex/identity.hpp>
#include <yggr/container_ex/is_container.hpp>
#include <yggr/ppex/has_member_check.hpp>
#include <yggr/range_ex/range_value_ex.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/mpl/bool.hpp>

#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_union.hpp>
#include <boost/type_traits/is_enum.hpp>


namespace yggr
{ 
namespace charset
{

namespace detail
{

template<typename T>
struct c_str_result_maker
	: public mplex::identity<const T*>
{
};

template<typename T, bool chk_is_container = container_ex::is_container<T>::value>
struct c_str_result
	: public mplex::identity<const void*>
{
};

template<typename T>
struct c_str_result<T, true>
	: public c_str_result_maker<typename range_ex::range_value_ex<T>::type>
{
};

template<typename T>
struct c_str_result<T*, false>
	: public c_str_result_maker<T>
{
};


template<typename T>
struct c_str_charable
	: public
		boost::mpl::bool_
		<
			((!(boost::is_class<T>::value
				|| boost::is_pointer<T>::value
				|| boost::is_floating_point<T>::value
				|| boost::is_reference<T>::value
				|| boost::is_union<T>::value
				|| boost::is_enum<T>::value))
			|| (boost::is_integral<T>::value))
		>::type
{
};


YGGR_PP_HAS_ANY_MEMBER_FOO_DEF(c_str, c_str)

template<typename T>
struct c_str_helper_impl
{
public:
	typedef const void* result_type;

public:
	template<typename TAny> inline
	result_type operator()(const TAny&) const
	{
		return static_cast<const void*>(0);
	}

};

} // namespace detail

template<typename String> inline
typename
	boost::enable_if
	<
		detail::has_any_c_str<String>, 
		typename detail::c_str_result<String>::type
	>::type
	c_str(const String& str)
{
	return str.c_str();
}

template<typename String> inline
typename
	boost::disable_if
	<
		detail::has_any_c_str<String>, 
		typename detail::c_str_helper_impl<String>::result_type
	>::type
	c_str(const String& str)
{
	typedef String now_string_type;
	typedef detail::c_str_helper_impl<now_string_type> helper_type;

	helper_type helper;
	return helper(str);
}

template<typename Char> YGGR_CONSTEXPR_OR_INLINE
typename boost::enable_if<detail::c_str_charable<Char>, const Char*>::type
	c_str(const Char* str)
{
	return str;
}


} // namespace charset
} // namespace yggr

#endif // __YGGR_CHARSET_C_STR_HPP__
