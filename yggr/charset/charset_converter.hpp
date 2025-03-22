//charset_converter.hpp

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

#ifndef __YGGR_CHARSET_CHARSET_CONVERTER_HPP__
#define __YGGR_CHARSET_CHARSET_CONVERTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>
#include <yggr/type_traits/is_native_char.hpp>

#include <yggr/utility/swap.hpp>

#include <yggr/mplex/static_assert.hpp>

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>

#include <yggr/charset/string_decl.hpp>
#include <yggr/charset/default_charset_config.hpp>
#include <yggr/charset/charset_base_foo.hpp>


#include <yggr/container/get_allocator.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/bool.hpp>

#include <cassert>
#include <iterator>
#include <algorithm>

#include <iconv.h>

/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! if you want to use wide-string store multibyte-string and the multibyte string length is odd,	!!!
!!!		will be more than some '\0'																!!!
!!! example: {L'a', L'b', L'c'}->utf8{'a', 'b', 'c'}->store_to ucs-2le wchar_t[2] = {'ab','c0'}		!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

namespace yggr
{
namespace charset
{

class charset_converter
	: private nonable::noncopyable,
		private nonable::nonmoveable
{

private:
	typedef charset_converter this_type;

//-------------------------------------------------------------------------
public:
	charset_converter(const char* src_charset, const char* dst_charset);

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	charset_converter(const Basic_String<char, Traits, Alloc>& src_charset,
						const char* dst_charset)
		: _src_charset(src_charset.data()), _dst_charset(dst_charset), _cd(iconv_t(-1))
	{
		assert(boost::size(src_charset));
		assert(dst_charset);
		this_type::prv_init_iconv(_src_charset, _dst_charset);
	}

	template<typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
	charset_converter(const char* src_charset,
						const Basic_String<char, Traits, Alloc>& dst_charset)
		: _src_charset(src_charset), _dst_charset(dst_charset.data()), _cd(iconv_t(-1))
	{
		assert(src_charset);
		assert(boost::size(dst_charset));
		this_type::prv_init_iconv(_src_charset, _dst_charset);
	}

	template<typename Traits1, typename Alloc1,
				typename Traits2, typename Alloc2,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String1,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String2 >
	charset_converter(const Basic_String1<char, Traits1, Alloc1>& src_charset,
						const Basic_String2<char, Traits2, Alloc2>& dst_charset)
		: _src_charset(src_charset.data()), _dst_charset(dst_charset.data()), _cd(iconv_t(-1))
	{
		assert(boost::size(src_charset));
		assert(boost::size(dst_charset));
		this_type::prv_init_iconv(_src_charset, _dst_charset);
	}

	~charset_converter(void);

public:
	inline bool is_validate(void) const
	{
		return !(_cd == iconv_t(-1));
	}

	// full conv, if return false means conv failed
	template<typename Char, typename OutString>
	typename
		boost::enable_if
		<
			is_native_char<Char>,
			bool
		>::type
		operator()(const Char* input, OutString& output) const
	{
		typedef Char in_char_type;
		typedef std::size_t in_size_type;
		typedef const Char* in_iter_type;

		typedef OutString out_string_type;
		typedef typename boost::range_value<out_string_type>::type out_char_type;
		typedef typename boost::range_difference<out_string_type>::type out_size_type;
		typedef typename boost::range_iterator<out_string_type>::type out_iter_type;

		if(!is_validate())
		{
			return false;
		}

		std::size_t ilen = charset_base_foo::strlen(input);
		std::size_t ilen_byte = ilen * sizeof(in_char_type);
		//std::size_t xchglen = charset_base_foo::xchglen(ilen);
		//std::size_t xchglen = charset_base_foo::xchglen(ilen_byte);
		std::size_t xchglen = 
			charset_base_foo::xchglen(
				charset_name_t<in_char_type>() == _src_charset?
					ilen : ilen_byte);

		std::size_t olen_byte = xchglen;

		if(charset_base_foo::E_space_expansion_rate == olen_byte)
		{
			out_string_type tmp(::yggr::get_allocator(output));
			::yggr::swap(output, tmp);
			return true;
		}

		out_string_type str(olen_byte, 0, ::yggr::get_allocator(output));
		if(this_type::prv_conv(reinterpret_cast<const char*>(&input[0]), ilen_byte,
								reinterpret_cast<char*>(&str[0]), olen_byte))
		{
			std::size_t use_byte = xchglen - olen_byte;
			out_iter_type start = boost::begin(str);
			std::advance(start, (use_byte + sizeof(out_char_type) - 1) / sizeof(out_char_type));
			out_iter_type end = boost::end(str);
			str.erase(start, end);

			::yggr::swap(output, str);
			return true;
		}
		else
		{
			out_string_type tmp(::yggr::get_allocator(output));
			::yggr::swap(output, tmp);
			return false;
		}
	}

	// full conv, if return false means conv failed
	template<typename Char, typename OutString>
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			bool
		>::type
		operator()(const Char* input, std::size_t n, OutString& output) const
	{
		typedef Char in_char_type;
		typedef std::size_t in_size_type;
		typedef const Char* in_iter_type;

		typedef OutString out_string_type;
		typedef typename boost::range_value<out_string_type>::type out_char_type;
		typedef typename boost::range_difference<out_string_type>::type out_size_type;
		typedef typename boost::range_iterator<out_string_type>::type out_iter_type;


		if(!is_validate())
		{
			return false;
		}

		std::size_t ilen_byte = n * sizeof(in_char_type);
		//std::size_t xchglen = charset_base_foo::xchglen(n);
		//std::size_t xchglen = charset_base_foo::xchglen(ilen_byte);
		std::size_t xchglen = 
			charset_base_foo::xchglen(
				charset_name_t<in_char_type>() == _src_charset?
					n : ilen_byte);
		std::size_t olen_byte = xchglen;

		if(charset_base_foo::E_space_expansion_rate == olen_byte)
		{
			out_string_type tmp(::yggr::get_allocator(output));
			::yggr::swap(output, tmp);
			return true;
		}

		out_string_type str(olen_byte, 0, ::yggr::get_allocator(output));
		if(this_type::prv_conv(reinterpret_cast<const char*>(&input[0]), ilen_byte,
								reinterpret_cast<char*>(&str[0]), olen_byte))
		{
			std::size_t use_byte = xchglen - olen_byte;
			out_iter_type start = boost::begin(str);
			std::advance(start, (use_byte + sizeof(out_char_type) - 1) / sizeof(out_char_type));
			out_iter_type end = boost::end(str);
			str.erase(start, end);

			::yggr::swap(output, str);
			return true;
		}
		else
		{
			out_string_type tmp(::yggr::get_allocator(output));
			::yggr::swap(output, tmp);
			return false;
		}
	}

	// full conv, if return false means conv failed
	template<typename InString, typename OutString>
	bool operator()(const InString& input, OutString& output) const
	{
		typedef InString in_string_type;
		typedef typename boost::range_value<in_string_type>::type in_char_type;
		typedef typename boost::range_difference<in_string_type>::type in_size_type;
		typedef typename boost::range_iterator<in_string_type>::type in_iter_type;

		typedef OutString out_string_type;
		typedef typename boost::range_value<out_string_type>::type out_char_type;
		typedef typename boost::range_difference<out_string_type>::type out_size_type;
		typedef typename boost::range_iterator<out_string_type>::type out_iter_type;

		if(!is_validate())
		{
			return false;
		}

		std::size_t ilen = boost::size(input);
		std::size_t ilen_byte = ilen * sizeof(in_char_type);
		//std::size_t xchglen = charset_base_foo::xchglen(ilen);
		//std::size_t xchglen = charset_base_foo::xchglen(ilen_byte);
		std::size_t xchglen = 
			charset_base_foo::xchglen(
				charset_name_t<in_char_type>() == _src_charset?
					ilen : ilen_byte);
		std::size_t olen_byte = xchglen;

		if(charset_base_foo::E_space_expansion_rate == olen_byte)
		{
			out_string_type tmp(::yggr::get_allocator(output));
			::yggr::swap(output, tmp);
			return true;
		}

		out_string_type str(olen_byte, 0, ::yggr::get_allocator(output));
		if(this_type::prv_conv(reinterpret_cast<const char*>(&input[0]), ilen_byte,
								reinterpret_cast<char*>(&str[0]), olen_byte))
		{
			std::size_t use_byte = xchglen - olen_byte;
			out_iter_type start = boost::begin(str);
			std::advance(start, (use_byte + sizeof(out_char_type) - 1) / sizeof(out_char_type));
			out_iter_type end = boost::end(str);
			str.erase(start, end);

			::yggr::swap(output, str);
			return true;
		}
		else
		{
			out_string_type tmp(::yggr::get_allocator(output));
			::yggr::swap(output, tmp);
			return false;
		}
	}

	// support incomplete conversion, if return false, means conv not completed, need check ilen_byte and olen_byte
	template<typename InString, typename OutString>
	bool operator()(const InString& input, std::size_t& ilen_byte, OutString& output, std::size_t& olen_byte) const
	{
		typedef InString in_string_type;
		typedef typename in_string_type::value_type in_char_type;
		typedef typename in_string_type::size_type in_size_type;

		typedef OutString out_string_type;
		typedef typename out_string_type::value_type out_char_type;
		typedef typename out_string_type::size_type out_size_type;

		if(!is_validate())
		{
			return false;
		}

		if(charset_base_foo::E_space_expansion_rate == olen_byte)
		{
			out_string_type tmp(::yggr::get_allocator(output));
			::yggr::swap(output, tmp);
			return true;
		}

		out_string_type str(olen_byte, 0, ::yggr::get_allocator(output));
		ilen_byte = boost::size(input) * sizeof(in_char_type);
		bool bright = this_type::prv_conv(reinterpret_cast<const char*>(&input[0]), ilen_byte,
											reinterpret_cast<char*>(&str[0]), olen_byte);
		output.swap(str);

		return bright;
	}

	// support incomplete conversion, if return false, means conv not completed, need check ilen_byte and olen_byte
	inline bool operator()(const char* input, std::size_t& ilen_byte, char* output, std::size_t& olen_byte) const
	{
		return
			this_type::is_validate()
			&& this_type::prv_conv(input, ilen_byte, output, olen_byte);
	}

private:
	inline void prv_init_iconv(const char* src_charset, const char* dst_charset)
	{
		_cd = iconv_open(dst_charset, src_charset);
		assert(_cd != iconv_t(-1));
	}

	inline bool prv_conv(const char* input, std::size_t& ilen_byte, char* output, std::size_t& olen_byte) const
	{
		assert(is_validate());

		if(!olen_byte)
		{
			return true;
		}
#	if defined(_LIBICONV_VERSION) && (_LIBICONV_VERSION <= 0x0109)
		const char **pin = &input;
#	else
		char **pin = const_cast<char**>(&input);
#	endif //_LIBICONV_VERSION

		char **pout = &output;
		memset(output, 0, sizeof(char) * olen_byte);

		return !(static_cast<size_t>(-1) == iconv(_cd, pin, &ilen_byte, pout, &olen_byte));
	}

private:
	const char* _src_charset;
	const char* _dst_charset;
	iconv_t _cd;
};

} // charset
} // namespace yggr

#endif //__YGGR_CHARSET_CHARSET_CONVERTER_HPP__
