//utf8_foo.hpp

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

#ifndef __YGGR_CHARSET_UTF8_FOO_HPP__
#define __YGGR_CHARSET_UTF8_FOO_HPP__

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>
#include <yggr/move/move.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/charset/string.hpp>

#include <yggr/ppex/typedef.hpp>
#include <yggr/charset/str_wstr_converter.hpp>
#include <yggr/mplex/iterator_to_value_t.hpp>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace charset
{

class utf8_foo : private nonable::noncreateable
{
private:
	typedef utf8_foo this_type;
public:

	//---------------utf8_char------------------------------
	static size_type charlen(const char c);
	static size_type charlen(const char* first);
	static size_type charlen(const wchar_t* first);

	template<typename Iterator>
	static size_type charlen(Iterator start)
	{
		typedef Iterator iter_type;
		typedef typename mplex::iterator_to_value_t<iter_type>::type val_type;

		const val_type& val = *start;
		return this_type::charlen((const char*)&val);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static size_type charlen(const Basic_String<Char, Traits, Alloc>& c)
	{
		if(c.empty())
		{
			return 0;
		}
		return this_type::charlen(c.c_str());
	}

	static bool is_only_char(char src);
	static bool is_only_char(const char* src);
	static bool is_only_char(const wchar_t* src);

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static bool is_only_char(const Basic_String<Char, Traits, Alloc>& src)
	{
		return this_type::is_only_char(src.c_str());
	}

	//-------------------utf8_string----------------------

	static size_type strlen(const char* str, size_type n = size_type(-1));
	static size_type strlen(const wchar_t* str, size_type n = size_type(-1));
	static size_type strlen(const char* start, const char* last);
	static size_type strlen(const wchar_t* start, const wchar_t* last);

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	static size_type strlen(const Basic_String<Char, Traits, Alloc>& str)
	{
		if(str.empty())
		{
			return 0;
		}
		return this_type::strlen((const char*)&str[0], str.size() * sizeof(Char));
	}



	template<typename InputIter>
	static size_type strlen(InputIter start, InputIter last)
	{
		typedef typename mplex::iterator_to_value_t<InputIter>::type char_type;
		size_type size = last - start;
		if(size == 0)
		{
			return 0;
		}

		assert(start < last);
		return this_type::strlen((const char*)(&(*start)), size * sizeof(char_type));
	}

	static bool is_utf8_string(const char* str);
	static bool is_utf8_string(const wchar_t* str);

	template<typename String>
	static bool is_utf8_string(const String& str)
	{
		if(str.empty())
		{
			return 0;
		}
		return this_type::is_utf8_string((const char*)&str[0]);
	}

	static const char* get_charset_name(const char* str);
	static const char* get_charset_name(const wchar_t* str);

	template<typename Char, typename Traits, typename Alloc>
	static const char* get_charset_name(const basic_string<Char, Traits, Alloc>& str)
	{
		return this_type::is_utf8_string(str)?
				IN_CHARSET_NAME
				: default_charset_config::get_default_string_charset_name< Char >();
	}
};

} // namespace charset
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif //__YGGR_CHARSET_UTF8_FOO_HPP__
