// string_member_var_get.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_STRING_MEMBER_VAR_GET_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_STRING_MEMBER_VAR_GET_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/charset/string_converter.hpp>
#include <yggr/charset/make_utf8_string_t_from_allocator.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/container_ex/allocator_traits.hpp>
#include <yggr/container_ex/is_allocator.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

using yggr::charset::make_utf8_string_t_from_allocator;

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

#define YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME( __type__, __val__, __foo__, _ ) \
	template<typename ...Args> inline \
	const __type__& __foo__(BOOST_FWD_REF(Args)... args) const { return __val__; } \
	\
	template<typename String, typename ...Args> inline \
	typename boost::enable_if<boost::is_same<String, __type__>, const String&>::type \
		__foo__(BOOST_FWD_REF(Args)... args) const { return __val__; }

#else

#define YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME( __type__, __val__, __foo__, __n__ ) \
	BOOST_PP_EXPR_IF( __n__, template< ) \
		YGGR_PP_FOO_TYPES_DEF( __n__ ) \
	BOOST_PP_EXPR_IF( __n__, > ) inline \
	const __type__& __foo__( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		return __val__; } \
	\
	template< typename String \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename boost::enable_if<boost::is_same<String, __type__>, const String&>::type \
		__foo__( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		return __val__; }

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


#define YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF( __type__, __val__, __foo__) \
	template<typename String> inline \
	typename boost::disable_if<boost::is_same<String, __type__>, String>::type \
		__foo__(const string& charset_name \
					= charset::charset_name_t<String>()) const { \
		typedef String now_string_type; \
		return charset::string_converter::s_conv<now_string_type>(__val__, YGGR_STR_UTF8_STRING_CHARSET_NAME(), charset_name); } \
	\
	template<typename String, typename Alloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				boost::mpl::not_< boost::is_same<String, __type__> >, \
				container_ex::is_allocator<Alloc> >, \
			String>::type \
		__foo__(const Alloc& alloc, \
					const string& charset_name \
						= charset::charset_name_t<String>()) const { \
		typedef String now_string_type; \
		return charset::string_converter::s_conv<now_string_type>(__val__, alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME(), charset_name); }

#define YGGR_PP_C_MONGO_STRING_VIEW_MEMBER_GET( __foo__ ) \
	template<typename Alloc> inline \
	typename \
		boost::enable_if< \
			container_ex::is_allocator<Alloc>, \
			typename detail::make_utf8_string_t_from_allocator<Alloc>::type >::type \
		__foo__(const Alloc& alloc, const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const { \
		typedef typename detail::make_utf8_string_t_from_allocator<Alloc>::type now_inner_string_type; \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		return now_inner_string_type(this_type::__foo__(), alloc); } \
	\
	template<typename String> inline \
	String __foo__(const string& ret_charset_name = charset::charset_name_t<String>()) const { \
		typedef String now_string_type; \
		return this_type::__foo__().template str<now_string_type>(ret_charset_name); } \
	\
	template<typename String, typename Alloc> inline \
	typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type \
		__foo__(const Alloc& alloc, const string& ret_charset_name = charset::charset_name_t<String>()) const { \
		typedef String now_string_type; \
		return this_type::__foo__().template str<now_string_type>(alloc, ret_charset_name); }

#define YGGR_PP_C_MONGO_STRING_MEMBER_GET YGGR_PP_C_MONGO_STRING_VIEW_MEMBER_GET

#endif // __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_STRING_MEMBER_VAR_GET_HPP__
