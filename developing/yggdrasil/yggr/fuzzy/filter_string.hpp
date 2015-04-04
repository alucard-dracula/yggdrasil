//filter_string.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_FUZZY_FILTER_STRING_HPP__
#define __YGGR_FUZZY_FILTER_STRING_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/regular_parse/regex_parse.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#ifdef BOOST_HAS_ICU
#	include <yggr/regular_parse/u32regex_parse.hpp>
#	include <yggr/charset/utf8_string.hpp>
#endif //BOOST_HAS_ICU

/*
*	please all function use try catch of call
*	example:
*	try
*	{
*		filter_string::foo()
*	}
*	catch(filter_string::error_type)
*	{
*	}
*/

namespace yggr
{
namespace fuzzy
{

class filter_string
	: nonable::noncreateable
{
public:
	typedef boost::regex_error error_type;

private:
	typedef filter_string this_type;

private:
	template<typename Char, typename Nil_T = int>
	struct regex_string;

	template<typename Nil_T>
	struct regex_string<char, Nil_T>
	{
		inline static const char* space(void)
		{
			return " ";
		}

		inline static const char* fmt(void)
		{
			return "(?1.*)(?2.*)(?3.*|.*)";
		}

		inline static const char* str_re(void)
		{
			return "(^\\W+)|(\\W+$)|(\\W+)";
		}
	};

	template<typename Nil_T>
	struct regex_string<wchar_t,  Nil_T>
	{
		inline static const wchar_t* space(void)
		{
			return L" ";
		}

		inline static const wchar_t* fmt(void)
		{
			return L"(?1.*)(?2.*)(?3.*|.*)";
		}

		inline static const wchar_t* str_re(void)
		{
			return L"(^\\W+)|(\\W+$)|(\\W+)";
		}
	};

public:


	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String>
	static Basic_String<Char, Traits, Alloc>
		make_string(const Basic_String<Char, Traits, Alloc>& str)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<string_type>));
		string_type tstr(regex_string<Char>::space() + str + regex_string<Char>::space());
		string_type fmt(regex_string<Char>::fmt());
		string_type str_re(regex_string<Char>::str_re());

		return  yggr::regular_parse::regex_parse::replace(tstr, str_re, fmt);
	}

	template<typename Char>
	static std::basic_string<Char> make_string(const Char* str)
	{
		typedef std::basic_string<Char> string_type;
		return this_type::make_string<Char,
												std::char_traits<Char>,
												std::allocator<Char>,
												std::basic_string>
											(string_type(str));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static Basic_String<Char, Traits, Alloc>
		make_string(const Basic_String<Char, Traits, Alloc>& str,
							const Basic_String<Char, Traits, Alloc>& nre,
							const Basic_String<Char, Traits, Alloc>& nfmt)
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		BOOST_MPL_ASSERT((charset::is_not_utf8_string<string_type>));

		string_type tstr(regex_string<Char>::space() + str + regex_string<Char>::space());
		string_type fmt(nre);
		string_type str_re(nfmt);
		return  yggr::regular_parse::regex_parse::replace(tstr, str_re, fmt);
	}

	template<typename Char>
	static std::basic_string<Char>
		make_string(const Char* str, const Char* nre, const Char* nfmt)
	{
		typedef std::basic_string<Char> string_type;
		return this_type::make_string<Char,
											std::char_traits<Char>,
											std::allocator<Char>,
											std::basic_string>
											(string_type(str), string_type(nre), string_type(nfmt));
	}

#ifdef BOOST_HAS_ICU

	inline static charset::utf8_string
		make_u32_string(const charset::utf8_string& str)
	{
		typedef charset::utf8_string string_type;
		string_type fmt(regex_string<char>::fmt());
		string_type str_re(regex_string<char>::str_re());
		string_type tstr = regex_string<char>::space() + str + regex_string<char>::space();

		return yggr::regular_parse::u32regex_parse::replace(tstr, str_re, fmt);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String>
	static charset::utf8_string
		make_u32_string(const Basic_String<Char, Traits, Alloc>& str)
	{
		typedef charset::utf8_string string_type;
		string_type fmt(regex_string<Char>::fmt());
		string_type str_re(regex_string<Char>::str_re());
		string_type tstr(str);
		tstr = regex_string<Char>::space() + tstr + regex_string<Char>::space();

		return yggr::regular_parse::u32regex_parse::replace(tstr, str_re, fmt);
	}

	template<typename Char>
	static charset::utf8_string
			make_u32_string(const Char* str)
	{
		typedef std::basic_string<Char> string_type;
		return this_type::make_u32_string<Char,
												std::char_traits<Char>,
												std::allocator<Char>,
												std::basic_string>
												(string_type(str));
	}

	template<typename Char1, typename Traits1, typename Alloc1,
				typename Char2, typename Traits2, typename Alloc2,
				typename Char3, typename Traits3, typename Alloc3,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String>
	static charset::utf8_string
		make_u32_string(const Basic_String<Char1, Traits1, Alloc1>& str,
													const Basic_String<Char2, Traits2, Alloc2>& nre,
													const Basic_String<Char3, Traits3, Alloc3>& nfmt)
	{
		typedef charset::utf8_string string_type;
		string_type fmt(nfmt);
		string_type str_re(nre);
		string_type tstr(str);
		tstr = regex_string<Char1>::space() + tstr + regex_string<Char1>::space();

		return yggr::regular_parse::u32regex_parse::replace(tstr, str_re, fmt);
	}

	template<typename Char1, typename Char2, typename Char3>
	static charset::utf8_string
		make_u32_string(const Char1* str, const Char2* nre, const Char3* nfmt)
	{
		typedef std::basic_string<Char1> string1_type;
		typedef std::basic_string<Char2> string2_type;
		typedef std::basic_string<Char3> string3_type;

		return this_type::make_u32_string<Char1, std::char_traits<Char1>, std::allocator<Char1>,
													Char2, std::char_traits<Char2>, std::allocator<Char2>,
													Char3, std::char_traits<Char3>, std::allocator<Char3>,
													std::basic_string>
													(string1_type(str), string2_type(nre), string3_type(nfmt));
	}
#endif // BOOST_HAS_ICU

};

} // namespace fuzzy
} // namespace yggr

#endif //__YGGR_FUZZY_FILTER_STRING_HPP__
