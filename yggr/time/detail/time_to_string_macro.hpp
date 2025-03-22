// time_to_string_macro.hpp

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

#ifndef __YGGR_TIME_DETAIL_TIME_TO_STRING_MACRO_HPP__
#define __YGGR_TIME_DETAIL_TIME_TO_STRING_MACRO_HPP__

#include <yggr/time/time_config.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/generalized_string.hpp>
#include <yggr/charset/make_utf8_string_t_from_allocator.hpp>

#include <boost/utility/enable_if.hpp>

// __default_format__ e.g. ::yggr::time::time_config::default_time_format<now_base_char_type>(), 
// __handler_foo_name__ e.g. ::yggr::time::time_formatting_utc

// YGGR_PP_TIME_TIME_TO_STRING_PRO_MEMBER_FOO
#define YGGR_PP_TIME_TIME_TO_STRING_PRO_MEMBER_FOO( __default_format__, __handler_foo_name__, __foo_name__ ) \
	template<typename String, typename Fmt> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::charset::is_generalized_string_t<Fmt> >, \
			String >::type \
		__foo_name__(const Fmt& format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Fmt)) const { \
		typedef String now_string_type; \
		typedef typename ::yggr::charset::basic_string_getter<now_string_type>::type now_base_string_type; \
		typedef typename ::yggr::range_ex::range_value_ex<now_base_string_type>::type now_base_char_type; \
		now_string_type ret_str; \
		if(charset::generalized_string_empty(format)) { \
			__handler_foo_name__( \
				ret_str, \
				__default_format__<now_base_char_type>(), \
				this_type::bridge_time(), ret_charset_name, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_base_char_type)); } \
		else { \
			__handler_foo_name__(ret_str, format, this_type::bridge_time(), ret_charset_name, fmt_charset_name); } \
		return YGGR_MOVE_RET(now_string_type, ret_str); } \
	\
	template<typename String, typename Fmt, typename Alloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::charset::is_generalized_string_t<Fmt>, \
				::yggr::container_ex::is_allocator<Alloc> >, \
			String >::type \
		__foo_name__(const Fmt& format, const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Fmt)) const { \
		typedef String now_string_type; \
		typedef typename ::yggr::charset::basic_string_getter<now_string_type>::type now_base_string_type; \
		typedef typename ::yggr::range_ex::range_value_ex<now_base_string_type>::type now_base_char_type; \
		now_string_type ret_str(alloc); \
		if(charset::generalized_string_empty(format)) { \
			__handler_foo_name__( \
				ret_str, \
				__default_format__<now_base_char_type>(), \
				this_type::bridge_time(), ret_charset_name, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_base_char_type)); } \
		else { \
			__handler_foo_name__(ret_str, format, this_type::bridge_time(), ret_charset_name, fmt_charset_name); } \
		return YGGR_MOVE_RET(now_string_type, ret_str); }

//YGGR_PP_TIME_TIME_TO_STRING_MEMBER_FOO
#define YGGR_PP_TIME_TIME_TO_STRING_MEMBER_FOO(__handler_foo_name__, __foo_name__) \
	inline ::yggr::utf8_string __foo_name__(void) const { \
		return __handler_foo_name__< ::yggr::utf8_string>(static_cast<const char*>(0)); } \
	\
	inline ::yggr::utf8_string __foo_name__(const char* format, \
									const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
									const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char)) const { \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		return \
			__handler_foo_name__< ::yggr::utf8_string>( \
				format, YGGR_STR_UTF8_STRING_CHARSET_NAME(), fmt_charset_name); } \
	\
	template<typename Alloc> inline \
	typename \
		boost::enable_if< \
			::yggr::container_ex::is_allocator<Alloc>, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type >::type \
		__foo_name__(const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char)) const { \
		typedef typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type ret_type; \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		return \
			__handler_foo_name__<ret_type>( \
				static_cast<const char*>(0), alloc, \
				YGGR_STR_UTF8_STRING_CHARSET_NAME(), fmt_charset_name); } \
	\
	template<typename Alloc> inline \
	typename \
		boost::enable_if< \
			::yggr::container_ex::is_allocator<Alloc>, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type >::type \
		__foo_name__(const char* format, const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char)) const { \
		typedef typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type ret_type; \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		return \
			__handler_foo_name__<ret_type>( \
				format, alloc, \
				 YGGR_STR_UTF8_STRING_CHARSET_NAME(), fmt_charset_name); } \
	\
	template<typename Char> inline \
	typename boost::enable_if< ::yggr::is_native_char<Char>, ::yggr::utf8_string>::type \
		__foo_name__(const Char* format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const { \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		return \
			__handler_foo_name__< ::yggr::utf8_string>( \
				format, YGGR_STR_UTF8_STRING_CHARSET_NAME(), fmt_charset_name); } \
	\
	template<typename Char, typename Alloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::is_native_char<Char>, \
				::yggr::container_ex::is_allocator<Alloc> >, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type >::type \
		__foo_name__(const Char* format, const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const { \
		typedef typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type ret_type; \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		return \
			__handler_foo_name__<ret_type>( \
				format, alloc, \
				YGGR_STR_UTF8_STRING_CHARSET_NAME(), fmt_charset_name); } \
	\
	template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString, \
				typename Char, typename Traits, typename Alloc> inline \
	typename boost::enable_if< ::yggr::charset::is_string_t< BasicString<Char, Traits, Alloc> >, ::yggr::utf8_string>::type \
		__foo_name__(const BasicString<Char, Traits, Alloc>& format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = \
						::yggr::charset::charset_name_t< BasicString<Char, Traits, Alloc> >()) const { \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		return \
			__handler_foo_name__< ::yggr::utf8_string>( \
				format, YGGR_STR_UTF8_STRING_CHARSET_NAME(), fmt_charset_name); } \
	\
	template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString, \
				typename Char, typename Traits, typename Alloc, \
				typename RetAlloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<BasicString<Char, Traits, Alloc> >, \
				::yggr::container_ex::is_allocator<RetAlloc> >, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<RetAlloc>::type >::type \
		__foo_name__(const BasicString<Char, Traits, Alloc>& format, const RetAlloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = \
						::yggr::charset::charset_name_t< BasicString<Char, Traits, Alloc> >()) const { \
		typedef typename ::yggr::charset::make_utf8_string_t_from_allocator<RetAlloc>::type ret_type; \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		return \
			__handler_foo_name__<ret_type>( \
				format, alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME(), fmt_charset_name); } \
	\
	template<template<typename _Char, typename _Traits> class BasicStringView, \
				typename Char, typename Traits> inline \
	typename boost::enable_if< ::yggr::charset::is_string_view_t< BasicStringView<Char, Traits> >, ::yggr::utf8_string>::type \
		__foo_name__(const BasicStringView<Char, Traits>& format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< BasicStringView<Char, Traits> >()) const { \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		return \
			__handler_foo_name__< ::yggr::utf8_string>( \
				format, YGGR_STR_UTF8_STRING_CHARSET_NAME(), fmt_charset_name); } \
	\
	template<template<typename _Char, typename _Traits> class BasicStringView, \
				typename Char, typename Traits, \
				typename RetAlloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_view_t<BasicStringView<Char, Traits> >, \
				::yggr::container_ex::is_allocator<RetAlloc> >, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<RetAlloc>::type >::type \
		__foo_name__(const BasicStringView<Char, Traits>& format, const RetAlloc& alloc, \
					const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
					const ::yggr::string& fmt_charset_name = \
						::yggr::charset::charset_name_t< BasicStringView<Char, Traits> >()) const { \
		typedef typename ::yggr::charset::make_utf8_string_t_from_allocator<RetAlloc>::type ret_type; \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		return \
			__handler_foo_name__<ret_type>( \
				format, alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME(), fmt_charset_name); } \
	\
	template<typename T1, typename T2> inline \
	::yggr::utf8_string __foo_name__(const ::yggr::charset::string_charset_helper<T1, T2>& format, \
								const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const { \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		return \
			__handler_foo_name__< ::yggr::utf8_string>( \
				format.first, YGGR_STR_UTF8_STRING_CHARSET_NAME(), format.second); } \
	\
	template<typename T1, typename T2, typename Alloc> inline \
	typename \
		boost::enable_if< \
			::yggr::container_ex::is_allocator<Alloc>, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type >::type \
		__foo_name__(const ::yggr::charset::string_charset_helper<T1, T2>& format, const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const { \
		typedef typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type ret_type; \
		assert(ret_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME()); \
		return \
			__handler_foo_name__<ret_type>( \
				format.first, alloc, \
				YGGR_STR_UTF8_STRING_CHARSET_NAME(), format.second); } \
	\
	template<typename String> inline \
	typename \
		boost::enable_if< \
			::yggr::charset::is_string_t<String>, \
			String >::type \
		__foo_name__(void) const { \
		typedef String ret_type; \
		typedef typename ::yggr::charset::basic_string_getter<ret_type>::type ret_base_type; \
		typedef typename ::yggr::range_ex::range_value_ex<ret_base_type>::type ret_char_type; \
		return __handler_foo_name__<ret_type>(static_cast<const ret_char_type*>(0)); } \
	\
	template<typename String> inline \
	typename \
		boost::enable_if< \
			::yggr::charset::is_string_t<String>, \
			String >::type \
		__foo_name__(const char* format, \
					const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
					const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char)) const { \
		typedef String ret_type; \
		return __handler_foo_name__<ret_type>(format, ret_charset_name, fmt_charset_name); } \
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
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< \
								typename \
									::yggr::range_ex::range_value_ex< \
										typename \
											::yggr::charset::basic_string_getter< \
												String \
											>::type \
									>::type \
							>()) const { \
		typedef String ret_type; \
		typedef typename ::yggr::charset::basic_string_getter<ret_type>::type ret_base_type; \
		typedef typename ::yggr::range_ex::range_value_ex<ret_base_type>::type ret_char_type; \
		return \
			__handler_foo_name__<ret_type>( \
				static_cast<const ret_char_type*>(0), alloc, \
				ret_charset_name, fmt_charset_name); } \
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
		return __handler_foo_name__<ret_type>(format, alloc, ret_charset_name, fmt_charset_name); } \
	\
	template<typename String, typename Char> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::is_native_char<Char> >, \
			String >::type \
		__foo_name__(const Char* format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const { \
		typedef String ret_type; \
		return __handler_foo_name__<ret_type>(format, ret_charset_name, fmt_charset_name); } \
	\
	template<typename String, typename Char, typename Alloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::is_native_char<Char>, \
				::yggr::container_ex::is_allocator<Alloc> >, \
			String >::type \
		__foo_name__(const Char* format, const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) const { \
		typedef String ret_type; \
		return __handler_foo_name__<ret_type>(format, alloc, ret_charset_name, fmt_charset_name); } \
	\
	template<typename String, \
				template<typename _Char, typename _Traits, typename _Alloc> class BasicString, \
				typename Char, typename Traits, typename Alloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::charset::is_string_t<BasicString<Char, Traits, Alloc> > >, \
 			String >::type \
		__foo_name__(const BasicString<Char, Traits, Alloc>& format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< BasicString<Char, Traits, Alloc> >()) const { \
		typedef String ret_type; \
		return __handler_foo_name__<ret_type>(format, ret_charset_name, fmt_charset_name); } \
	\
	template<typename String,\
				template<typename _Char, typename _Traits, typename _Alloc> class BasicString, \
				typename Char, typename Traits, typename Alloc, typename RetAlloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::charset::is_string_t< BasicString<Char, Traits, Alloc> >, \
				::yggr::container_ex::is_allocator<RetAlloc> >, \
			String >::type \
		__foo_name__(const BasicString<Char, Traits, Alloc>& format, const RetAlloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< BasicString<Char, Traits, Alloc> >() ) const { \
		typedef String ret_type; \
		return __handler_foo_name__<ret_type>(format, alloc, ret_charset_name, fmt_charset_name); } \
	\
	template<typename String, \
				template<typename _Char, typename _Traits> class BasicStringView, \
				typename Char, typename Traits> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::charset::is_string_view_t<BasicStringView<Char, Traits> > >, \
 			String >::type \
		__foo_name__(const BasicStringView<Char, Traits>& format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< BasicStringView<Char, Traits> >()) const { \
		typedef String ret_type; \
		return __handler_foo_name__<ret_type>(format, ret_charset_name, fmt_charset_name); } \
	\
	template<typename String,\
				template<typename _Char, typename _Traits> class BasicStringView, \
				typename Char, typename Traits, typename RetAlloc> inline \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::charset::is_string_view_t< BasicStringView<Char, Traits> >, \
				::yggr::container_ex::is_allocator<RetAlloc> >, \
			String >::type \
		__foo_name__(const BasicStringView<Char, Traits>& format, const RetAlloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< BasicStringView<Char, Traits> >() ) const { \
		typedef String ret_type; \
		return __handler_foo_name__<ret_type>(format, alloc, ret_charset_name, fmt_charset_name); } \
	\
	template<typename String, typename T1, typename T2> inline \
	typename boost::enable_if< ::yggr::charset::is_string_t<String>, String>::type \
		__foo_name__(const ::yggr::charset::string_charset_helper<T1, T2>& format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String)) const { \
		typedef String ret_type; \
		return __handler_foo_name__<ret_type>(format.first, ret_charset_name, format.second); } \
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
		return __handler_foo_name__<ret_type>(format.first, alloc, ret_charset_name, format.second); }

// YGGR_PP_TIME_TIME_TO_STRING_STATIC_MEMBER_FOO
#define YGGR_PP_TIME_TIME_TO_STRING_STATIC_MEMBER_FOO(__handler_foo_name__, __foo_name__) \
	inline static ::yggr::utf8_string __foo_name__(void) { \
		this_type tmp; \
		return tmp.__handler_foo_name__(); } \
	\
	inline static ::yggr::utf8_string __foo_name__(const char* format, \
													const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
													const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char)) { \
		this_type tmp; \
		return tmp.__handler_foo_name__(format, ret_charset_name, fmt_charset_name); } \
	\
	template<typename Alloc> inline \
	static \
	typename \
		boost::enable_if< \
			::yggr::container_ex::is_allocator<Alloc>, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type >::type \
		__foo_name__(const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char)) { \
		this_type tmp; \
		return tmp.__handler_foo_name__(alloc, ret_charset_name, fmt_charset_name); } \
	\
	template<typename Alloc> inline \
	static \
	typename \
		boost::enable_if< \
			::yggr::container_ex::is_allocator<Alloc>, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type >::type \
		__foo_name__(const char* format, const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char)) { \
		this_type tmp; \
		return tmp.__handler_foo_name__(format, alloc, ret_charset_name, fmt_charset_name); } \
	\
	template<typename Char> inline \
	static \
	typename boost::enable_if< ::yggr::is_native_char<Char>, ::yggr::utf8_string>::type \
		__foo_name__(const Char* format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) { \
		this_type tmp; \
		return tmp.__handler_foo_name__(format, ret_charset_name, fmt_charset_name); } \
	\
	template<typename Char, typename Alloc> inline \
	static \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::is_native_char<Char>, \
				::yggr::container_ex::is_allocator<Alloc> >, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type >::type \
		__foo_name__(const Char* format, const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) { \
		this_type tmp; \
		return tmp.__handler_foo_name__(format, alloc, ret_charset_name, fmt_charset_name); } \
	\
	template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString, \
				typename Char, typename Traits, typename Alloc> inline \
	static \
	typename boost::enable_if< ::yggr::charset::is_string_t< BasicString<Char, Traits, Alloc> >, ::yggr::utf8_string>::type \
		__foo_name__(const BasicString<Char, Traits, Alloc>& format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< BasicString<Char, Traits, Alloc> >()) { \
		this_type tmp; \
		return tmp.__handler_foo_name__(format, ret_charset_name, fmt_charset_name); } \
	\
	template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString, \
				typename Char, typename Traits, typename Alloc, \
				typename RetAlloc> inline \
	static \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<BasicString<Char, Traits, Alloc> >, \
				::yggr::container_ex::is_allocator<RetAlloc> >, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<RetAlloc>::type >::type \
		__foo_name__(const BasicString<Char, Traits, Alloc>& format, const RetAlloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = \
						::yggr::charset::charset_name_t< BasicString<Char, Traits, Alloc> >()) { \
		this_type tmp; \
		return tmp.__handler_foo_name__(format, alloc, ret_charset_name, fmt_charset_name); } \
	\
	template<template<typename _Char, typename _Traits> class BasicStringView, \
				typename Char, typename Traits> inline \
	static \
	typename boost::enable_if< \
		::yggr::charset::is_string_view_t<BasicStringView<Char, Traits> >, \
		::yggr::utf8_string>::type \
		__foo_name__(const BasicStringView<Char, Traits>& format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< BasicStringView<Char, Traits> >()) { \
		this_type tmp; \
		return tmp.__handler_foo_name__(format, ret_charset_name, fmt_charset_name); } \
	\
	template<template<typename _Char, typename _Traits> class BasicStringView, \
				typename Char, typename Traits, \
				typename RetAlloc> inline \
	static \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_view_t<BasicStringView<Char, Traits> >, \
				::yggr::container_ex::is_allocator<RetAlloc> >, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<RetAlloc>::type >::type \
		__foo_name__(const BasicStringView<Char, Traits>& format, const RetAlloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME(), \
						const ::yggr::string& fmt_charset_name = \
						::yggr::charset::charset_name_t< BasicStringView<Char, Traits> >()) { \
		this_type tmp; \
		return tmp.__handler_foo_name__(format, alloc, ret_charset_name, fmt_charset_name); } \
	\
	template<typename T1, typename T2> inline \
	static \
	::yggr::utf8_string __foo_name__(const ::yggr::charset::string_charset_helper<T1, T2>& format, \
										const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) { \
		this_type tmp; \
		return tmp.__handler_foo_name__(format, ret_charset_name); } \
	\
	template<typename T1, typename T2, typename Alloc> inline \
	static \
	typename \
		boost::enable_if< \
			::yggr::container_ex::is_allocator<Alloc>, \
			typename ::yggr::charset::make_utf8_string_t_from_allocator<Alloc>::type >::type \
		__foo_name__(const ::yggr::charset::string_charset_helper<T1, T2>& format, const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) { \
		this_type tmp; \
		return tmp.__handler_foo_name__(format, alloc, ret_charset_name); } \
	\
	template<typename String> inline \
	static \
	typename \
		boost::enable_if< \
			::yggr::charset::is_string_t<String>, \
			String >::type \
		__foo_name__(void) { \
		typedef String ret_type; \
		this_type tmp; \
		return tmp.__handler_foo_name__<ret_type>(); } \
	\
	template<typename String> inline \
	static \
	typename \
		boost::enable_if< \
			::yggr::charset::is_string_t<String>, \
			String >::type \
		__foo_name__(const char* format, \
					const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
					const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char)) { \
		typedef String ret_type; \
		this_type tmp; \
		return tmp.__handler_foo_name__<ret_type>(format, ret_charset_name, fmt_charset_name); } \
	\
	template<typename String, typename Alloc> inline \
	static \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::container_ex::is_allocator<Alloc> >, \
			String >::type \
		__foo_name__(const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< \
								typename \
									::yggr::range_ex::range_value_ex< \
										typename \
											::yggr::charset::basic_string_getter< \
												String \
											>::type \
									>::type \
							>()) { \
		typedef String ret_type; \
		this_type tmp; \
		return tmp.__handler_foo_name__<ret_type>(alloc, ret_charset_name, fmt_charset_name); } \
	\
	template<typename String, typename Alloc> inline \
	static \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::container_ex::is_allocator<Alloc> >, \
			String >::type \
		__foo_name__(const char* format, const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char)) { \
		typedef String ret_type; \
		this_type tmp; \
		return tmp.__handler_foo_name__<ret_type>(format, alloc, ret_charset_name, fmt_charset_name); } \
	\
	template<typename String, typename Char> inline \
	static \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::is_native_char<Char> >, \
			String >::type \
		__foo_name__(const Char* format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) { \
		typedef String ret_type; \
		this_type tmp; \
		return tmp.__handler_foo_name__<ret_type>(format, ret_charset_name, fmt_charset_name); } \
	\
	template<typename String, typename Char, typename Alloc> inline \
	static \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::is_native_char<Char>, \
				::yggr::container_ex::is_allocator<Alloc> >, \
			String >::type \
		__foo_name__(const Char* format, const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char)) { \
		typedef String ret_type; \
		this_type tmp; \
		return tmp.__handler_foo_name__<ret_type>(format, alloc, ret_charset_name, fmt_charset_name); } \
	\
	template<typename String, \
				template<typename _Char, typename _Traits, typename _Alloc> class BasicString, \
				typename Char, typename Traits, typename Alloc> inline \
	static \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String >, \
				::yggr::charset::is_string_t<BasicString<Char, Traits, Alloc> > >, \
 			String >::type \
		__foo_name__(const BasicString<Char, Traits, Alloc>& format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< BasicString<Char, Traits, Alloc> >() ) { \
		typedef String ret_type; \
		this_type tmp; \
		return tmp.__handler_foo_name__<ret_type>(format, ret_charset_name, fmt_charset_name); } \
	\
	template<typename String, \
				template<typename _Char, typename _Traits, typename _Alloc> class BasicString, \
				typename Char, typename Traits, typename Alloc, typename RetAlloc> inline \
	static \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::charset::is_string_t<BasicString<Char, Traits, Alloc> >, \
				::yggr::container_ex::is_allocator<RetAlloc> >, \
			String >::type \
		__foo_name__(const BasicString<Char, Traits, Alloc>& format, const RetAlloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< BasicString<Char, Traits, Alloc> >() ) { \
		typedef String ret_type; \
		this_type tmp; \
		return tmp.__handler_foo_name__<ret_type>(format, alloc, ret_charset_name, fmt_charset_name); } \
	\
	template<typename String, \
				template<typename _Char, typename _Traits> class BasicStringView, \
				typename Char, typename Traits> inline \
	static \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String >, \
				::yggr::charset::is_string_view_t<BasicStringView<Char, Traits> > >, \
 			String >::type \
		__foo_name__(const BasicStringView<Char, Traits>& format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String), \
						const ::yggr::string& fmt_charset_name = \
							::yggr::charset::charset_name_t< BasicStringView<Char, Traits> >() ) { \
		typedef String ret_type; \
		this_type tmp; \
		return tmp.__handler_foo_name__<ret_type>(format, ret_charset_name, fmt_charset_name); } \
	\
	template<typename String, \
				template<typename _Char, typename _Traits> class BasicStringView, \
				typename Char, typename Traits, typename RetAlloc> inline \
	static \
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
							::yggr::charset::charset_name_t< BasicStringView<Char, Traits> >() ) { \
		typedef String ret_type; \
		this_type tmp; \
		return tmp.__handler_foo_name__<ret_type>(format, alloc, ret_charset_name, fmt_charset_name); } \
	\
	template<typename String, typename T1, typename T2> inline \
	static \
	typename boost::enable_if< ::yggr::charset::is_string_t<String>, String>::type \
		__foo_name__(const ::yggr::charset::string_charset_helper<T1, T2>& format, \
						const ::yggr::string& ret_charset_name = YGGR_STR_STRING_DEFAULT_CHARSET_NAME(String)) { \
		typedef String ret_type; \
		this_type tmp; \
		return tmp.__handler_foo_name__<ret_type>(format, ret_charset_name); } \
	\
	template<typename String, typename T1, typename T2, typename Alloc> inline \
	static \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::charset::is_string_t<String>, \
				::yggr::container_ex::is_allocator<Alloc> >, \
			String >::type \
		__foo_name__(const ::yggr::charset::string_charset_helper<T1, T2>& format, const Alloc& alloc, \
						const ::yggr::string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) { \
		typedef String ret_type; \
		this_type tmp; \
		return tmp.__handler_foo_name__<ret_type>(format, alloc, ret_charset_name); }

#endif // __YGGR_TIME_DETAIL_TIME_TO_STRING_MACRO_HPP__