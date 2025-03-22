//str_wstr_converter.hpp

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

#ifndef __YGGR_CHARSET_STR_WSTR_COVERTER_HPP__
#define __YGGR_CHARSET_STR_WSTR_COVERTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/charset/default_charset_config.hpp>
#include <yggr/charset/charset_foo.hpp>

#include <yggr/ppex/symbols.hpp>

#include <boost/utility/enable_if.hpp>

#include <memory>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace charset
{

class str_wstr_converter
	: private nonable::noncreateable
{
private:
	typedef str_wstr_converter this_type;

public:

	//warning not suppot std to boost
	template<typename Char> inline
	static
		typename 
			boost::enable_if
			<
				is_native_char<Char>,
				basic_string<typename other_char_chr_wchr<Char>::type>
			>::type
		s_conv(const Char* src,
				const string& src_charset_name = charset_name_t<Char>(),
				const string& dst_charset_name = charset_name_t<typename other_char_chr_wchr<Char>::type>())
	{
		typedef basic_string<typename other_char_chr_wchr<Char>::type> dst_type;

		return charset_foo::s_xchg<dst_type>(src, src_charset_name, dst_charset_name);
	}

	template<typename Char> inline
	static
		typename 
			boost::enable_if
			<
				is_native_char<Char>,
				basic_string<typename other_char_chr_wchr<Char>::type>
			>::type
		s_conv(const Char* src, size_type n,
				const string& src_charset_name = charset_name_t<Char>(),
				const string& dst_charset_name = charset_name_t<typename other_char_chr_wchr<Char>::type>())
	{
		typedef basic_string<typename other_char_chr_wchr<Char>::type> dst_type;

		return charset_foo::s_xchg<dst_type>(src, n, src_charset_name, dst_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template< typename _Char, typename _Traits, typename _Alloc > class Basic_String > inline
	static
		typename
			boost::enable_if
			<
				is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
				typename other_string_str_wstr< Basic_String<Char, Traits, Alloc> >::type
			>::type
		s_conv(const Basic_String<Char, Traits, Alloc>& src,
					const string& src_charset_name = charset_name_t<Char>(),
					const string& dst_charset_name = charset_name_t<typename other_char_chr_wchr<Char>::type>())
	{
		typedef typename other_string_str_wstr< Basic_String<Char, Traits, Alloc> >::type dst_type;
		return charset_foo::s_xchg<dst_type>(src, src_charset_name, dst_charset_name);
	}
};

} // namespace charset
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif //__YGGR_CHARSET_STR_WSTR_COVERTER_HPP__
