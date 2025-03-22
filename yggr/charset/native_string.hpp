//native_string.hpp

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

#ifndef __YGGR_CHARSET_NATIVE_STRING_HPP__
#define __YGGR_CHARSET_NATIVE_STRING_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/utf8_string.hpp>

#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>

namespace yggr
{
namespace charset
{

// native array 
template<typename Dst_Char, typename Char, std::size_t size> inline
typename 
	boost::enable_if_c
	<
		(0 == (size * sizeof(Char)) % sizeof(Dst_Char)), 
		Dst_Char(&)[size * sizeof(Char) / sizeof(Dst_Char)]
	>::type
	native_string(Char(&str)[size])
{
	typedef Dst_Char(&dst_type)[size * sizeof(Char) / sizeof(Dst_Char)];
	return reinterpret_cast<dst_type>(str);
}

template<typename Dst_Char, typename Char, std::size_t size> inline
typename 
	boost::enable_if_c
	<
		(0 == (size * sizeof(Char)) % sizeof(Dst_Char)), 
		Dst_Char const(&)[size * sizeof(Char) / sizeof(Dst_Char)]
	>::type
	native_string(Char const (&str)[size])
{
	typedef Dst_Char const (&dst_type)[size * sizeof(Char) / sizeof(Dst_Char)];
	return reinterpret_cast<dst_type>(str);
}

template<typename Char, std::size_t size> inline
typename
	boost::enable_if
	<
		is_convertible_char<Char>,
		Char(&)[size]
	>::type
	native_string(Char (&str)[size])
{
	return str;
}

template<typename Char, std::size_t size> inline
typename
	boost::enable_if
	<
		is_convertible_char<Char>,
		Char const(&)[size]
	>::type
	native_string(Char const (&str)[size])
{
	return str;
}

// native const char*

template<typename Dst_Char, typename Char> inline 
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			is_convertible_char<Dst_Char>,
			is_convertible_char<Char>
		>,
		Dst_Char*
	>::type
	native_string(Char*& str)
{
	return reinterpret_cast<Dst_Char*>(str);
}

template<typename Dst_Char, typename Char> inline 
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			is_convertible_char<Dst_Char>,
			is_convertible_char<Char>
		>,
		const Dst_Char*
	>::type
	native_string(const Char*& str)
{
	return reinterpret_cast<const Dst_Char*>(str);
}

template<typename CharPtr> inline 
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::is_pointer<CharPtr>,
			is_convertible_char<typename boost::remove_pointer<CharPtr>::type>
		>,
		CharPtr
	>::type
	native_string(CharPtr& str)
{
	return str;
}

template<typename CharPtr> inline 
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::is_pointer<CharPtr>,
			is_convertible_char<typename boost::remove_pointer<CharPtr>::type>
		>,
		const CharPtr
	>::type
	native_string(const CharPtr& str)
{
	return str;
}

// basic_string

//basic_string_ref
// template<typename _Char, typename _Traits, typename _Alloc> class BasicString must first tpl_arg
template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
			typename Char, typename Traits, typename Alloc> inline
typename 
	boost::enable_if
	<
		is_basic_string_t< BasicString<Char, Traits, Alloc> >,
		BasicString<Char, Traits, Alloc>&
	>::type
	native_string(BasicString<Char, Traits, Alloc>& str)
{
	return str;
}

template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
			typename Char, typename Traits, typename Alloc, typename ArgAlloc> inline
typename 
	boost::enable_if
	<
		is_basic_string_t< BasicString<Char, Traits, Alloc> >,
		BasicString<Char, Traits, Alloc>&
	>::type
	native_string(BasicString<Char, Traits, Alloc>& str, const ArgAlloc&)
{
	return str;
}

template<typename String,
			template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
			typename Char, typename Traits, typename Alloc> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			is_basic_string_t< BasicString<Char, Traits, Alloc> >,
			boost::is_same<String, BasicString<Char, Traits, Alloc> >
		>,
		String&
	>::type
	native_string(BasicString<Char, Traits, Alloc>& str)
{
	return str;
}

template<typename String,
			template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
			typename Char, typename Traits, typename Alloc> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			is_basic_string_t< BasicString<Char, Traits, Alloc> >,
			is_isomeric_same_string<String, BasicString<Char, Traits, Alloc> >
		>,
		String&
	>::type
	native_string(BasicString<Char, Traits, Alloc>& str)
{
	return reinterpret_cast<String&>(str);
}

template<typename String,
			template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
			typename Char, typename Traits, typename Alloc, typename ArgAlloc> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			is_basic_string_t< BasicString<Char, Traits, Alloc> >,
			boost::is_same<String, BasicString<Char, Traits, Alloc> >
		>,
		String&
	>::type
	native_string(BasicString<Char, Traits, Alloc>& str, const ArgAlloc&)
{
	return str;
}

//basic_string_cref

template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
			typename Char, typename Traits, typename Alloc > inline
typename 
	boost::enable_if
	<
		is_basic_string_t< BasicString<Char, Traits, Alloc> >,
		const BasicString<Char, Traits, Alloc>&
	>::type
	native_string(const BasicString<Char, Traits, Alloc>& str)
{
	return str;
}

template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
			typename Char, typename Traits, typename Alloc, typename ArgAlloc> inline
typename 
	boost::enable_if
	<
		is_basic_string_t< BasicString<Char, Traits, Alloc> >,
		const BasicString<Char, Traits, Alloc>&
	>::type
	native_string(const BasicString<Char, Traits, Alloc>& str, const ArgAlloc&)
{
	return str;
}

template<typename String,
			template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
			typename Char, typename Traits, typename Alloc > inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			is_basic_string_t< BasicString<Char, Traits, Alloc> >,
			boost::is_same<String, BasicString<Char, Traits, Alloc> >
		>,
		const String&
	>::type
	native_string(const BasicString<Char, Traits, Alloc>& str)
{
	return str;
}

template<typename String,
			template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
			typename Char, typename Traits, typename Alloc> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			is_basic_string_t< BasicString<Char, Traits, Alloc> >,
			is_isomeric_same_string<String, BasicString<Char, Traits, Alloc> >
		>,
		const String&
	>::type
	native_string(const BasicString<Char, Traits, Alloc>& str)
{
	return reinterpret_cast<const String&>(str);
}

template<typename String,
			template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
			typename Char, typename Traits, typename Alloc, typename ArgAlloc> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			is_basic_string_t< BasicString<Char, Traits, Alloc> >,
			boost::is_same<String, BasicString<Char, Traits, Alloc> >
		>,
		const String&
	>::type
	native_string(const BasicString<Char, Traits, Alloc>& str, const ArgAlloc&)
{
	return str;
}

template<typename String,
			template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
			typename Char, typename Traits, typename Alloc, typename ArgAlloc> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			is_basic_string_t< BasicString<Char, Traits, Alloc> >,
			is_isomeric_same_string<String, BasicString<Char, Traits, Alloc> >
		>,
		const String&
	>::type
	native_string(const BasicString<Char, Traits, Alloc>& str, const ArgAlloc&)
{
	return reinterpret_cast<const String&>(str);
}


template<typename String,
			template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
			typename Char, typename Traits, typename Alloc> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			is_basic_string_t< BasicString<Char, Traits, Alloc> >,
			boost::mpl::not_
			< 
				boost::mpl::or_
				<
					boost::is_same<String, BasicString<Char, Traits, Alloc> >,
					is_isomeric_same_string<String, BasicString<Char, Traits, Alloc> >
				>
			>
		>,
		String
	>::type
	native_string(const BasicString<Char, Traits, Alloc>& str)
{
	typedef String string_type;
	typedef typename boost::range_value<string_type>::type value_type;

	if(!str.empty())
	{
		std::size_t byte_size = str.size() * sizeof(Char);
		std::size_t size = (byte_size + sizeof(value_type) - 1) / sizeof(value_type);

		string_type ret(size, value_type(0));
		memcpy(&ret[0], &str[0], byte_size);
		return ret;
	}
	else
	{
		return string_type();
	}
}

template<typename String,
			template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
			typename Char, typename Traits, typename Alloc, typename ArgAlloc> inline
typename 
	boost::enable_if
	<
		boost::mpl::and_
		<
			is_basic_string_t< BasicString<Char, Traits, Alloc> >,
			boost::mpl::not_
			< 
				boost::mpl::or_
				<
					boost::is_same<String, BasicString<Char, Traits, Alloc> >,
					is_isomeric_same_string<String, BasicString<Char, Traits, Alloc> >
				> 
			>
		>,
		String
	>::type
	native_string(const BasicString<Char, Traits, Alloc>& str, const ArgAlloc& alloc)
{
	typedef String string_type;
	typedef typename boost::range_value<string_type>::type value_type;
	//typedef typename string_type::allocator_type chk_type;
	//BOOST_MPL_ASSERT((boost::is_same<chk_type, String_Alloc>));

	if(!str.empty())
	{
		std::size_t byte_size = str.size() * sizeof(Char);
		std::size_t size = (byte_size + sizeof(value_type) - 1) / sizeof(value_type);

		string_type ret(size, value_type(0), alloc);
		memcpy(&ret[0], &str[0], byte_size);
		return ret;
	}
	else
	{
		return string_type(alloc);
	}
}

// utf8_char
// utf8_char_ref
template<typename Char, typename Traits, typename Alloc> inline
typename utf8_char_impl<Char, Traits, Alloc>::base_type&
	native_string(utf8_char_impl<Char, Traits, Alloc>& str)
{
	return str.org_str();
}

template<typename String, typename Char, typename Traits, typename Alloc> inline
typename 
	boost::enable_if
	<
		boost::is_same<String, typename utf8_char_impl<Char, Traits, Alloc>::base_type >,
		String&
	>::type
	native_string(utf8_char_impl<Char, Traits, Alloc>& str)
{
	return str.org_str();
}

template<typename String, typename Char, typename Traits, typename Alloc> inline
typename 
	boost::enable_if
	<
		is_isomeric_same_string<String, typename utf8_char_impl<Char, Traits, Alloc>::base_type >,
		String&
	>::type
	native_string(utf8_char_impl<Char, Traits, Alloc>& str)
{
	return str.template org_str<String>();
}

template<typename String, typename Char, typename Traits, typename Alloc, typename ArgAlloc> inline
typename 
	boost::enable_if
	<
		boost::is_same<String, typename utf8_char_impl<Char, Traits, Alloc>::base_type >,
		String&
	>::type
	native_string(utf8_char_impl<Char, Traits, Alloc>& str, const ArgAlloc&)
{
	return str.org_str();
}

template<typename String, typename Char, typename Traits, typename Alloc, typename ArgAlloc> inline
typename 
	boost::enable_if
	<
		is_isomeric_same_string<String, typename utf8_char_impl<Char, Traits, Alloc>::base_type >,
		String&
	>::type
	native_string(utf8_char_impl<Char, Traits, Alloc>& str, const ArgAlloc&)
{
	return str.template org_str<String>();
}

// utf8_char_cref
template<typename Char, typename Traits, typename Alloc> inline
const typename utf8_char_impl<Char, Traits, Alloc>::base_type&
	native_string(const utf8_char_impl<Char, Traits, Alloc>& str)
{
	return str.org_str();
}

template<typename String, typename Char, typename Traits, typename Alloc> inline
typename 
	boost::enable_if
	<
		boost::is_same<String, typename utf8_char_impl<Char, Traits, Alloc>::base_type >,
		const String&
	>::type
	native_string(const utf8_char_impl<Char, Traits, Alloc>& str)
{
	return str.org_str();
}

template<typename String, typename Char, typename Traits, typename Alloc> inline
typename 
	boost::enable_if
	<
		is_isomeric_same_string<String, typename utf8_char_impl<Char, Traits, Alloc>::base_type >,
		const String&
	>::type
	native_string(const utf8_char_impl<Char, Traits, Alloc>& str)
{
	return str.template org_str<String>();
}

template<typename String, typename Char, typename Traits, typename Alloc, typename ArgAlloc> inline
typename 
	boost::enable_if
	<
		boost::is_same<String, typename utf8_char_impl<Char, Traits, Alloc>::base_type >,
		const String&
	>::type
	native_string(const utf8_char_impl<Char, Traits, Alloc>& str, const ArgAlloc&)
{
	return str.org_str();
}

template<typename String, typename Char, typename Traits, typename Alloc, typename ArgAlloc> inline
typename 
	boost::enable_if
	<
		is_isomeric_same_string<String, typename utf8_char_impl<Char, Traits, Alloc>::base_type >,
		const String&
	>::type
	native_string(const utf8_char_impl<Char, Traits, Alloc>& str, const ArgAlloc&)
{
	return str.template org_str<String>();
}

template<typename String, typename Char, typename Traits, typename Alloc> inline
typename 
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::is_same<String, typename utf8_char_impl<Char, Traits, Alloc>::base_type >,
			is_isomeric_same_string<String, typename utf8_char_impl<Char, Traits, Alloc>::base_type >
		>,
		String
	>::type
	native_string(const utf8_char_impl<Char, Traits, Alloc>& str)
{
	return str.template org_str<String>();
}

template<typename String, 
			typename Char, typename Traits, typename Alloc,
			typename ArgAlloc > inline
typename 
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::is_same<String, typename utf8_char_impl<Char, Traits, Alloc>::base_type >,
			is_isomeric_same_string<String, typename utf8_char_impl<Char, Traits, Alloc>::base_type >
		>,
		String
	>::type
	native_string(const utf8_char_impl<Char, Traits, Alloc>& str, const ArgAlloc& alloc)
{
	return str.template org_str<String>(alloc);
}

// utf8_char_view

template<typename Char, typename Traits> inline
typename basic_utf8_char_view<Char, Traits>::base_type
	native_string(const basic_utf8_char_view<Char, Traits>& str)
{
	return str;
}

template<typename String, typename Char, typename Traits> inline
String native_string(const basic_utf8_char_view<Char, Traits>& str)
{
	return str.template org_str<String>();
}

template<typename String, 
			typename Char, typename Traits, typename Alloc,
			typename ArgAlloc> inline
String native_string(const utf8_char_impl<Char, Traits, Alloc>& str, const ArgAlloc& alloc)
{
	return str.template org_str<String>(alloc);
}

// basic_utf8_char_reference
template<typename Utf8String> inline
typename 
	detail::utf8_char_ref_org_str_helper
	<
		typename Utf8String::base_type,
		is_utf8_char_t<Utf8String>::value 
	>::nonconst_result_type
	native_string(basic_utf8_char_reference<Utf8String>& str)
{
	return str.org_str();
}

template<typename Utf8String, typename ArgAlloc> inline
typename 
	detail::utf8_char_ref_org_str_helper
	<
		typename Utf8String::base_type,
		is_utf8_char_t<Utf8String>::value 
	>::nonconst_result_type
	native_string(basic_utf8_char_reference<Utf8String>& str, const ArgAlloc)
{
	return str.org_str();
}

template<typename Utf8String> inline
typename 
	detail::utf8_char_ref_org_str_helper
	<
		typename Utf8String::base_type,
		is_utf8_char_t<Utf8String>::value 
	>::const_result_type
	native_string(const basic_utf8_char_reference<Utf8String>& str)
{
	return str.org_str();
}

template<typename Utf8String, typename ArgAlloc> inline
typename 
	detail::utf8_char_ref_org_str_helper
	<
		typename Utf8String::base_type,
		is_utf8_char_t<Utf8String>::value 
	>::const_result_type
	native_string(const basic_utf8_char_reference<Utf8String>& str, const ArgAlloc&)
{
	return str.org_str();
}

template<typename String, typename Utf8String, typename ArgAlloc> inline
String native_string(const basic_utf8_char_reference<Utf8String>& str, const ArgAlloc& alloc)
{
	return str.template org_str<String>(alloc);
}

// basic_utf8_char_const_reference
template<typename Utf8String> inline
typename 
	detail::utf8_char_cref_org_str_helper
	<
		typename Utf8String::base_type,
		is_utf8_char_t<Utf8String>::value 
	>::const_result_type
	native_string(const basic_utf8_char_const_reference<Utf8String>& str)
{
	return str.org_str();
}

template<typename Utf8String, typename ArgAlloc> inline
typename 
	detail::utf8_char_cref_org_str_helper
	<
		typename Utf8String::base_type,
		is_utf8_char_t<Utf8String>::value 
	>::const_result_type
	native_string(const basic_utf8_char_const_reference<Utf8String>& str, const ArgAlloc&)
{
	return str.org_str();
}

template<typename String, typename Utf8String> inline
String native_string(const basic_utf8_char_const_reference<Utf8String>& str)
{
	return str.template org_str<String>();
}

template<typename String, typename Utf8String, typename ArgAlloc> inline
String native_string(const basic_utf8_char_const_reference<Utf8String>& str, const ArgAlloc& alloc)
{
	return str.template org_str<String>(alloc);
}

//utf8_string

template<typename Char, typename Traits, typename Alloc> inline
typename utf8_string_impl<Char, Traits, Alloc>::base_type&
	native_string(utf8_string_impl<Char, Traits, Alloc>& str)
{
	return str.org_str();
}

template<typename Char, typename Traits, typename Alloc, typename ArgAlloc> inline
typename utf8_string_impl<Char, Traits, Alloc>::base_type&
	native_string(utf8_string_impl<Char, Traits, Alloc>& str, const ArgAlloc&)
{
	return str.org_str();
}

template<typename String, typename Char, typename Traits, typename Alloc> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			boost::is_same<String, utf8_string_impl<Char, Traits, Alloc> >,
			boost::is_same<String, typename utf8_string_impl<Char, Traits, Alloc>::base_type>
		>,
		String&
	>::type
	native_string(utf8_string_impl<Char, Traits, Alloc>& str)
{
	return str.template org_str<String>();
}

template<typename String, typename Char, typename Traits, typename Alloc> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			is_isomeric_same_string<String, utf8_string_impl<Char, Traits, Alloc> >,
			is_isomeric_same_string<String, typename utf8_string_impl<Char, Traits, Alloc>::base_type>
		>,
		String&
	>::type
	native_string(utf8_string_impl<Char, Traits, Alloc>& str)
{
	return reinterpret_cast<String&>(str.template org_str<String>());
}

template<typename String, typename Char, typename Traits, typename Alloc> inline
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::mpl::or_
			<
				boost::is_same<String, utf8_string_impl<Char, Traits, Alloc> >,
				boost::is_same<String, typename utf8_string_impl<Char, Traits, Alloc>::base_type>
			>,
			boost::mpl::or_
			<
				is_isomeric_same_string<String, utf8_string_impl<Char, Traits, Alloc> >,
				is_isomeric_same_string<String, typename utf8_string_impl<Char, Traits, Alloc>::base_type>
			>
		>,
		String
	>::type
	native_string(utf8_string_impl<Char, Traits, Alloc>& str)
{
	return str.template org_str<String>();
}

template<typename String, typename Char, typename Traits, typename Alloc, typename ArgAlloc> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			boost::is_same<String, utf8_string_impl<Char, Traits, Alloc> >,
			boost::is_same<String, typename utf8_string_impl<Char, Traits, Alloc>::base_type>
		>,
		String&
	>::type
	native_string(utf8_string_impl<Char, Traits, Alloc>& str, const ArgAlloc&)
{
	return str.template org_str<String>();
}

template<typename String, typename Char, typename Traits, typename Alloc, typename ArgAlloc> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			is_isomeric_same_string<String, utf8_string_impl<Char, Traits, Alloc> >,
			is_isomeric_same_string<String, typename utf8_string_impl<Char, Traits, Alloc>::base_type>
		>,
		String&
	>::type
	native_string(utf8_string_impl<Char, Traits, Alloc>& str, const ArgAlloc&)
{
	return reinterpret_cast<String&>(str.template org_str<String>());
}

template<typename String, typename Char, typename Traits, typename Alloc, typename ArgAlloc> inline
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::mpl::or_
			<
				boost::is_same<String, utf8_string_impl<Char, Traits, Alloc> >,
				boost::is_same<String, typename utf8_string_impl<Char, Traits, Alloc>::base_type>
			>,
			boost::mpl::or_
			<
				is_isomeric_same_string<String, utf8_string_impl<Char, Traits, Alloc> >,
				is_isomeric_same_string<String, typename utf8_string_impl<Char, Traits, Alloc>::base_type>
			>
		>,
		String
	>::type
	native_string(utf8_string_impl<Char, Traits, Alloc>& str, const ArgAlloc& alloc)
{
	return str.template org_str<String>(alloc);
}

template<typename String, typename Char, typename Traits, typename Alloc> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			boost::is_same<String, utf8_string_impl<Char, Traits, Alloc> >,
			boost::is_same<String, typename utf8_string_impl<Char, Traits, Alloc>::base_type>
		>,
		const String&
	>::type
	native_string(const utf8_string_impl<Char, Traits, Alloc>& str)
{
	return str.template org_str<String>();
}

// utf8_string_cref

template<typename Char, typename Traits, typename Alloc> inline
const typename utf8_string_impl<Char, Traits, Alloc>::base_type&
	native_string(const utf8_string_impl<Char, Traits, Alloc>& str)
{
	return str.org_str();
}

template<typename Char, typename Traits, typename Alloc, typename ArgAlloc> inline
const typename utf8_string_impl<Char, Traits, Alloc>::base_type&
	native_string(const utf8_string_impl<Char, Traits, Alloc>& str, const ArgAlloc&)
{
	return str.org_str();
}

template<typename String, typename Char, typename Traits, typename Alloc> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			is_isomeric_same_string<String, utf8_string_impl<Char, Traits, Alloc> >,
			is_isomeric_same_string<String, typename utf8_string_impl<Char, Traits, Alloc>::base_type>
		>,
		const String&
	>::type
	native_string(const utf8_string_impl<Char, Traits, Alloc>& str)
{
	return reinterpret_cast<const String&>(str.template org_str<String>());
}

template<typename String, typename Char, typename Traits, typename Alloc> inline
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::mpl::or_
			<
				boost::is_same<String, utf8_string_impl<Char, Traits, Alloc> >,
				boost::is_same<String, typename utf8_string_impl<Char, Traits, Alloc>::base_type>
			>,
			boost::mpl::or_
			<
				is_isomeric_same_string<String, utf8_string_impl<Char, Traits, Alloc> >,
				is_isomeric_same_string<String, typename utf8_string_impl<Char, Traits, Alloc>::base_type>
			>
		>,
		String
	>::type
	native_string(const utf8_string_impl<Char, Traits, Alloc>& str)
{
	return str.template org_str<String>();
}

template<typename String, typename Char, typename Traits, typename Alloc, typename ArgAlloc> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			boost::is_same<String, utf8_string_impl<Char, Traits, Alloc> >,
			boost::is_same<String, typename utf8_string_impl<Char, Traits, Alloc>::base_type>
		>,
		const String&
	>::type
	native_string(const utf8_string_impl<Char, Traits, Alloc>& str, const ArgAlloc&)
{
	return str.template org_str<String>();
}

template<typename String, typename Char, typename Traits, typename Alloc, typename ArgAlloc> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			is_isomeric_same_string<String, utf8_string_impl<Char, Traits, Alloc> >,
			is_isomeric_same_string<String, typename utf8_string_impl<Char, Traits, Alloc>::base_type>
		>,
		const String&
	>::type
	native_string(const utf8_string_impl<Char, Traits, Alloc>& str, const ArgAlloc&)
{
	return reinterpret_cast<const String&>(str.template org_str<String>());
}

template<typename String, typename Char, typename Traits, typename Alloc, typename ArgAlloc> inline
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::mpl::or_
			<
				boost::is_same<String, utf8_string_impl<Char, Traits, Alloc> >,
				boost::is_same<String, typename utf8_string_impl<Char, Traits, Alloc>::base_type>
			>,
			boost::mpl::or_
			<
				is_isomeric_same_string<String, utf8_string_impl<Char, Traits, Alloc> >,
				is_isomeric_same_string<String, typename utf8_string_impl<Char, Traits, Alloc>::base_type>
			>
		>,
		String
	>::type
	native_string(const utf8_string_impl<Char, Traits, Alloc>& str, const ArgAlloc& alloc)
{
	return str.template org_str<String>(alloc);
}

} // namespace charset
} // namespace yggr

#endif // __YGGR_CHARSET_NATIVE_STRING_HPP__