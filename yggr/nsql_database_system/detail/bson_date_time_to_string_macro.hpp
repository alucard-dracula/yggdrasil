// bson_date_time_to_string_macro.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_BSON_DATE_TIME_TO_STRING_MACRO_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_BSON_DATE_TIME_TO_STRING_MACRO_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/charset_base_foo.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/time/detail/time_to_string_macro.hpp>

#include <yggr/nsql_database_system/bson_date_time_format.hpp>
#include <yggr/nsql_database_system/bson_date_time_native_ex.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>

#include <cassert>

// don't using YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL, because std::strftime not support (const char*, size)
// YGGR_PP_BSON_DATE_TIME_TO_STRING_PRO_MEMBER_FOO
#define YGGR_PP_BSON_DATE_TIME_TO_STRING_PRO_MEMBER_FOO( __default_format__, __foo_name__ ) \
	inline ::yggr::utf8_string __foo_name__(const char* format = 0) const { \
		typedef ::yggr::utf8_string inner_string_type; \
		format = format? format : __default_format__(); \
		assert(::yggr::charset::charset_base_foo::strlen(format) < ::yggr::time::time_config::limit_format_length); \
		char buf[::yggr::time::time_config::default_string_buffer_length] = {0}; \
		::yggr::nsql_database_system::bson_date_time_native_ex::s_bson_date_format( \
			format, this_type::date(), buf, ::yggr::time::time_config::default_string_buffer_length); \
		return buf[0]? inner_string_type(buf, YGGR_STR_UTF8_STRING_CHARSET_NAME()) : inner_string_type(); } \
	\
	template<typename Alloc> inline \
	typename \
		boost::enable_if< \
			::yggr::container_ex::is_allocator<Alloc>, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type >::type \
		__foo_name__(const char* format, const Alloc& alloc) const { \
		typedef typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type inner_string_type; \
		format = format? format : __default_format__(); \
		assert(::yggr::charset::charset_base_foo::strlen(format) < ::yggr::time::time_config::limit_format_length); \
		char buf[::yggr::time::time_config::default_string_buffer_length] = {0}; \
		::yggr::nsql_database_system::bson_date_time_native_ex::s_bson_date_format( \
			format, this_type::date(), buf, ::yggr::time::time_config::default_string_buffer_length); \
		return buf[0]? inner_string_type(buf, alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()) : inner_string_type(alloc); }


// don't using YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL, because std::strftime not support (const char*, size)
//YGGR_PP_BSON_DATE_TIME_TO_STRING_MEMBER_FOO

#define YGGR_PP_BSON_DATE_TIME_TO_STRING_MEMBER_FOO(__handler_foo_name__, __foo_name__) \
	inline utf8_string __foo_name__(void) const { return __handler_foo_name__(); } \
	\
	inline utf8_string __foo_name__(const char* format, \
									const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
									const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char)) const { \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		return \
			fmt_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()? \
				__handler_foo_name__(format) \
				: __handler_foo_name__(format? utf8_string(format, fmt_charset_name).data() : format); } \
	\
	template<typename Alloc> inline \
	typename \
		boost::enable_if< \
			::yggr::container_ex::is_allocator<Alloc>, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type >::type \
		__foo_name__(const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME() ) const { \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		return __handler_foo_name__(static_cast<const char*>(0), alloc); } \
	\
	template<typename Alloc> inline \
	typename \
		boost::enable_if< \
			::yggr::container_ex::is_allocator<Alloc>, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type >::type \
		__foo_name__(const char* format, const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char)) const { \
		return \
			fmt_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()? \
				__handler_foo_name__(format, alloc) \
				: __handler_foo_name__( \
					format? utf8_string(format, fmt_charset_name).data() : format, alloc); } \
	\
	template<typename Char> inline \
	utf8_string __foo_name__(const Char* format, \
								const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
								const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const { \
		typedef Char* now_fmt_string_type; \
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type); \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		if(format) { \
			format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name))); \
			return __handler_foo_name__(str_fmt.data()); } \
		else { \
			return __handler_foo_name__(); } } \
	\
	template<typename Char, typename Alloc> inline \
	typename \
		boost::enable_if< \
			::yggr::container_ex::is_allocator<Alloc>, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type >::type \
		__foo_name__(const Char* format, const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const { \
		typedef Char* now_fmt_string_type; \
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type); \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		if(format) { \
			format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name))); \
			return __handler_foo_name__(str_fmt.data(), alloc); } \
		else { \
			return __handler_foo_name__(static_cast<const char*>(0), alloc); } } \
	\
	template<template<typename _Char, typename _Trtaits, typename _Alloc> class Basic_String, \
				typename Char, typename Traits, typename Alloc> inline \
	typename \
		boost::enable_if< \
			::yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> >, \
			utf8_string >::type \
		__foo_name__(const Basic_String<Char, Traits, Alloc>& format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< Basic_String<Char, Traits, Alloc> >() ) const { \
		typedef Basic_String<Char, Traits, Alloc> now_fmt_string_type; \
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type); \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name))); \
		return __handler_foo_name__(str_fmt.data()); } \
	\
	template<template<typename _Char, typename _Trtaits, typename _Alloc> class Basic_String, \
				typename Char, typename Traits, typename Alloc, typename RetAlloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> >, \
				::yggr::container_ex::is_allocator<RetAlloc> >, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<RetAlloc>::type >::type \
		__foo_name__(const Basic_String<Char, Traits, Alloc>& format, const RetAlloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< Basic_String<Char, Traits, Alloc> >()) const { \
		typedef Basic_String<Char, Traits, Alloc> now_fmt_string_type; \
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type); \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name))); \
		return __handler_foo_name__(str_fmt.data(), alloc); } \
	\
	template<template<typename _Char, typename _Trtaits> class BasicStringView, \
				typename Char, typename Traits> inline \
	typename \
		boost::enable_if< \
			::yggr::charset::is_string_view_t< BasicStringView<Char, Traits> >, \
			utf8_string >::type \
		__foo_name__(const BasicStringView<Char, Traits>& format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< BasicStringView<Char, Traits> >() ) const { \
		typedef BasicStringView<Char, Traits> now_fmt_string_type; \
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type); \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name))); \
		return __handler_foo_name__(str_fmt.data()); } \
	\
	template<template<typename _Char, typename _Trtaits> class BasicStringView, \
				typename Char, typename Traits, typename RetAlloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_view_t< BasicStringView<Char, Traits> >, \
				::yggr::container_ex::is_allocator<RetAlloc> >, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<RetAlloc>::type >::type \
		__foo_name__(const BasicStringView<Char, Traits>& format, const RetAlloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< BasicStringView<Char, Traits> >()) const { \
		typedef BasicStringView<Char, Traits> now_fmt_string_type; \
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type); \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name))); \
		return __handler_foo_name__(str_fmt.data(), alloc); } \
	\
	template<typename T1, typename T2> inline \
	utf8_string __foo_name__(const ::yggr::charset::string_charset_helper<T1, T2>& format, \
								const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const { \
		typedef ::yggr::charset::string_charset_helper<T1, T2> now_fmt_string_type; \
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type); \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format))); \
		return __handler_foo_name__(str_fmt.data()); } \
	\
	template<typename T1, typename T2, typename Alloc> inline \
	typename \
		boost::enable_if< \
			::yggr::container_ex::is_allocator<Alloc>, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type >::type \
		__foo_name__(const ::yggr::charset::string_charset_helper<T1, T2>& format, const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const { \
		typedef ::yggr::charset::string_charset_helper<T1, T2> now_fmt_string_type; \
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type); \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format))); \
		return __handler_foo_name__(str_fmt.data(), alloc); } \
	\
	template<typename String> inline \
	typename \
		boost::enable_if< \
			::yggr::charset::is_string_t<String>, \
			String >::type \
		__foo_name__(void) const { \
		typedef String ret_type; \
		return __handler_foo_name__(0).str<ret_type>(); } \
	\
	template<typename String> inline \
	typename \
		boost::enable_if< \
			::yggr::charset::is_string_t<String>, \
			String >::type \
		__foo_name__(const char* format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const { \
		typedef String ret_type; \
		return \
			fmt_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()? \
				__handler_foo_name__(format).str<ret_type>(ret_charset_name) \
				: __handler_foo_name__(format? utf8_string(format, fmt_charset_name).data() : format).str<ret_type>(ret_charset_name); } \
	\
	template<typename String, typename Alloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::container_ex::is_allocator<Alloc> >, \
			String >::type \
		__foo_name__(const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const { \
		typedef String ret_type; \
		return __handler_foo_name__(0).str<ret_type>(alloc, ret_charset_name); } \
	\
	template<typename String, typename Alloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::container_ex::is_allocator<Alloc> >, \
			String >::type \
		__foo_name__(const char* format, const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char)) const { \
		typedef String ret_type; \
		return \
			fmt_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()? \
				__handler_foo_name__(format).str<ret_type>(alloc, ret_charset_name) \
				: __handler_foo_name__(format? utf8_string(format, fmt_charset_name).data() : format).str<ret_type>(alloc, ret_charset_name); } \
	\
	template<typename String, typename Char> inline \
	typename \
		boost::enable_if< \
			::yggr::charset::is_string_t<String>, \
			String >::type \
		__foo_name__(const Char* format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const { \
		typedef String ret_type; \
		typedef Char* now_fmt_string_type; \
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type); \
		format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name))); \
		return __handler_foo_name__(str_fmt.data()).template str<ret_type>(ret_charset_name); } \
	\
	template<typename String, typename Char, typename Alloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::container_ex::is_allocator<Alloc> >, \
			String >::type \
		__foo_name__(const Char* format, const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const { \
		typedef String ret_type; \
		typedef Char* now_fmt_string_type; \
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type); \
		format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name))); \
		return __handler_foo_name__(str_fmt.data()).template str<ret_type>(alloc, ret_charset_name); } \
	\
	template<typename String, \
				template<typename _Char, typename _Trtaits, typename _Alloc> class Basic_String, \
				typename Char, typename Traits, typename Alloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t< String >, \
				::yggr::charset::is_string_t< Basic_String<Char, Traits, Alloc> > >, \
 			String >::type \
		__foo_name__(const Basic_String<Char, Traits, Alloc>& format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< Basic_String<Char, Traits, Alloc> >() ) const { \
		typedef String ret_type; \
		typedef Basic_String<Char, Traits, Alloc> now_fmt_string_type; \
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type); \
		format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name))); \
		return __handler_foo_name__(str_fmt.data()).template str<ret_type>(ret_charset_name); } \
	\
	template<typename String, \
				template<typename _Char, typename _Trtaits, typename _Alloc> class Basic_String, \
				typename Char, typename Traits, typename Alloc, typename RetAlloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::charset::is_string_t<Basic_String<Char, Traits, Alloc> >, \
				::yggr::container_ex::is_allocator<Alloc> >, \
			String >::type \
		__foo_name__(const Basic_String<Char, Traits, Alloc>& format, const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< Basic_String<Char, Traits, Alloc> >() ) const { \
		typedef String ret_type; \
		typedef Basic_String<Char, Traits, Alloc> now_fmt_string_type; \
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type); \
		format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name))); \
		return __handler_foo_name__(str_fmt.data()).template str<ret_type>(alloc, ret_charset_name); } \
	\
	template<typename String, \
				template<typename _Char, typename _Trtaits> class BasicStringView, \
				typename Char, typename Traits> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t< String >, \
				::yggr::charset::is_string_view_t< BasicStringView<Char, Traits> > >, \
 			String >::type \
		__foo_name__(const BasicStringView<Char, Traits>& format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< BasicStringView<Char, Traits> >() ) const { \
		typedef String ret_type; \
		typedef BasicStringView<Char, Traits> now_fmt_string_type; \
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type); \
		format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name))); \
		return __handler_foo_name__(str_fmt.data()).template str<ret_type>(ret_charset_name); } \
	\
	template<typename String, \
				template<typename _Char, typename _Trtaits> class BasicStringView, \
				typename Char, typename Traits, typename RetAlloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::charset::is_string_view_t<BasicStringView<Char, Traits> >, \
				::yggr::container_ex::is_allocator<RetAlloc> >, \
			String >::type \
		__foo_name__(const BasicStringView<Char, Traits>& format, const RetAlloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< BasicStringView<Char, Traits> >() ) const { \
		typedef String ret_type; \
		typedef BasicStringView<Char, Traits> now_fmt_string_type; \
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type); \
		format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format, fmt_charset_name))); \
		return __handler_foo_name__(str_fmt.data()).template str<ret_type>(alloc, ret_charset_name); } \
	\
	template<typename String, typename T1, typename T2> inline \
	typename boost::enable_if< ::yggr::charset::is_string_t<String>, String>::type \
		__foo_name__(const ::yggr::charset::string_charset_helper<T1, T2>& format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String)) const { \
		typedef String ret_type; \
		typedef ::yggr::charset::string_charset_helper<T1, T2> now_fmt_string_type; \
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type); \
		format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format))); \
		return __handler_foo_name__(str_fmt.data()).template str<ret_type>(ret_charset_name); } \
	\
	template<typename String, typename T1, typename T2, typename Alloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::container_ex::is_allocator<Alloc> >, \
			String >::type \
		__foo_name__(const ::yggr::charset::string_charset_helper<T1, T2>& format, const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const { \
		typedef String ret_type; \
		typedef ::yggr::charset::string_charset_helper<T1, T2> now_fmt_string_type; \
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_fmt_string_type, format_string_type); \
		format_string_type str_fmt((::yggr::charset::string_charset_helper_data(format))); \
		return __handler_foo_name__(str_fmt.data()).template str<ret_type>(alloc, ret_charset_name); }


namespace yggr
{
namespace nsql_database_system
{
namespace detail
{
} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_DETAIL_BSON_DATE_TIME_TO_STRING_MACRO_HPP__
