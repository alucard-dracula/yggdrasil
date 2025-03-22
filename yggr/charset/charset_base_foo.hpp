//charset_base_foo.hpp

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

#ifndef __YGGR_CHARSET_CHARSET_BASE_FOO_HPP__
#define __YGGR_CHARSET_CHARSET_BASE_FOO_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/nonable/noncreateable.hpp>

#include <yggr/charset/charset_type_traits.hpp>
#include <yggr/charset/convertible_traits.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/integral_c.hpp>

#include <algorithm>
#include <cstring>
#include <cassert>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace charset
{

class charset_base_foo
	: private nonable::noncreateable
{
public:

#ifdef YGGR_USE_UTF8_EX_MODE
	YGGR_STATIC_CONSTANT(u32, E_space_expansion_rate = 6);
#else
	YGGR_STATIC_CONSTANT(u32, E_space_expansion_rate = 4);
#endif // YGGR_USE_UTF8_EX_MODE

private:
	typedef charset_base_foo this_type;

public:
	//strlen, xchglen
	inline static size_type strlen(const char* str)
	{
		return std::strlen(str);
	}

	inline static size_type xchglen(const char* str)
	{
		return (std::strlen(str) + 1) * E_space_expansion_rate;
	}

#ifndef YGGR_NO_CWCHAR

	inline static size_type strlen(const wchar_t* str)
	{
		return std::wcslen(str);
	}

	inline static size_type xchglen(const wchar_t* str)
	{
		return (std::wcslen(str) + 1) * E_space_expansion_rate;
	}

#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CHAR8_T
	inline static size_type strlen(const ::yggr::c8* str)
	{
		typedef std::char_traits< ::yggr::c8> traits_type;
		return traits_type::length(str);
	}

	inline static size_type xchglen(const ::yggr::c8* str)
	{
		typedef std::char_traits< ::yggr::c8> traits_type;
		return (traits_type::length(str) + 1) * E_space_expansion_rate;
	}

#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_CHAR16_T
	inline static size_type strlen(const ::yggr::c16* str)
	{
		typedef std::char_traits< ::yggr::c16> traits_type;
		return traits_type::length(str);
	}

	inline static size_type xchglen(const ::yggr::c16* str)
	{
		typedef std::char_traits< ::yggr::c16> traits_type;
		return (traits_type::length(str) + 1) * E_space_expansion_rate;
	}

#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T
	inline static size_type strlen(const ::yggr::c32* str)
	{
		typedef std::char_traits< ::yggr::c32> traits_type;
		return traits_type::length(str);
	}

	inline static size_type xchglen(const ::yggr::c32* str)
	{
		typedef std::char_traits< ::yggr::c32> traits_type;
#ifdef YGGR_USE_UTF8_EX_MODE
		return (traits_type::length(str) + 1) * 2;
#else
		return (traits_type::length(str) + 1);
#endif // YGGR_USE_UTF8_EX_MODE
	}

#endif // YGGR_NO_CHAR32_T

	template<typename Char>
	static
	typename 
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		strlen(const Char* str)
	{
		typedef Char char_type;
		const char_type* p = str;
		for(;*p; ++p);
		return static_cast<size_type>(p - str);
	}

	template<typename Char>
	static
	typename 
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		strnlen(const Char* str, size_type n = -1)
	{
		typedef Char char_type;
		const char_type* p = str;
		for(;(*p) && (static_cast<size_type>(p - str) != n); ++p);
		return static_cast<size_type>(p - str);
	}

	template<typename Char> inline
	static
	typename 
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		xchglen(const Char* str)
	{
		typedef Char char_type;
		return (this_type::strlen(str) + 1) * E_space_expansion_rate;
	}

	template<typename Char> inline
	static
	typename 
		boost::enable_if
		<
			is_convertible_char<Char>,
			size_type
		>::type
		xchgnlen(const Char* str, size_type n = -1)
	{
		typedef Char char_type;
		return (this_type::strnlen(str, n) + 1) * E_space_expansion_rate;
	}
	
	YGGR_CONSTEXPR_OR_INLINE static size_type xchglen(size_type n)
	{
		return (n + 1) * E_space_expansion_rate;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> YGGR_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			size_type
		>::type
		xchglen(const Basic_String<Char, Traits, Alloc>& str)
	{
		return (str.size() + 1) * E_space_expansion_rate;
	}

	// normal_length
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	static
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			Basic_String<Char, Traits, Alloc>&
		>::type
		normal_length(Basic_String<Char, Traits, Alloc>& str)
	{
		typedef Char char_type;

		size_type pos = str.find_first_of(static_cast<char_type>(0));
		if(pos != str.npos)
		{
			str.erase(pos);
		}
		return str;
	}

	// strcmp_case
	inline static s_size_type strcmp_case(const char* l, const char* r)
	{
#	if defined(_MSC_VER) || defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		return _stricmp(l, r);
#	else
		return strcasecmp(l, r);
#	endif // _MSC_VER
	}

	static s_size_type strncmp_case(const char* l, const char* r, size_type len);

	template<typename Char>
	static
	typename 
		boost::enable_if
		<
			is_convertible_char<Char>,
			s_size_type
		>::type
		strcmp_case(const Char* l, const Char* r)
	{
		typedef boost::mpl::integral_c<Char, static_cast<Char>('a')> E_low_start_t;
		typedef boost::mpl::integral_c<Char, static_cast<Char>('z')> E_low_end_t;

		typedef boost::mpl::integral_c<Char, static_cast<Char>('A')> E_upper_start_t;
		typedef boost::mpl::integral_c<Char, static_cast<Char>('Z')> E_upper_end_t;

		typedef boost::mpl::integral_c<Char, (E_low_start_t::value - E_upper_start_t::value)> E_delta_t;

		BOOST_MPL_ASSERT((boost::mpl::bool_<(E_delta_t::value > 0)>));

		assert((l && r));

		if(l == r)
		{
			return 0;
		}

		size_type i = 0;
		bool ret = false;
		for(; (l[i]) && (r[i]) 
				&& (ret = 
					((l[i] == r[i])
					|| ((l[i] >= E_low_start_t::value && l[i] <= E_low_end_t::value) && l[i] - E_delta_t::value == r[i])
					|| ((l[i] >= E_upper_start_t::value && l[i] <= E_upper_end_t::value) && l[i] + E_delta_t::value == r[i]) )); ++i);
	
		return (ret) && (l[i] == r[i])? 0 : l[i] < r[i]? -1 : 1;
	}

	template<typename Char>
	static
	typename 
		boost::enable_if
		<
			is_convertible_char<Char>,
			s_size_type
		>::type
		strncmp_case(const Char* l, const Char* r, size_type len)
	{
		typedef boost::mpl::integral_c<Char, static_cast<Char>('a')> E_low_start_t;
		typedef boost::mpl::integral_c<Char, static_cast<Char>('z')> E_low_end_t;

		typedef boost::mpl::integral_c<Char, static_cast<Char>('A')> E_upper_start_t;
		typedef boost::mpl::integral_c<Char, static_cast<Char>('Z')> E_upper_end_t;

		typedef boost::mpl::integral_c<Char, (E_low_start_t::value - E_upper_start_t::value)> E_delta_t;

		BOOST_MPL_ASSERT((boost::mpl::bool_<(E_delta_t::value > 0)>));

		assert((l && r));

		if(l == r)
		{
			return 0;
		}

		size_type i = 0;
		bool ret = false;
		for(; (i != len)
				&& (l[i]) && (r[i]) 
				&& (ret = 
					((l[i] == r[i])
					|| ((l[i] >= E_low_start_t::value && l[i] <= E_low_end_t::value) && l[i] - E_delta_t::value == r[i])
					|| ((l[i] >= E_upper_start_t::value && l[i] <= E_upper_end_t::value) && l[i] + E_delta_t::value == r[i]) )); ++i);
	
		assert(i <= len);

		return (ret) && (i == len || l[i] == r[i])? 0 : l[i] < r[i]? -1 : 1;
	}
};

} // namespace charset
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif //__YGGR_CHARSET_CHARSET_BASE_FOO_HPP__
