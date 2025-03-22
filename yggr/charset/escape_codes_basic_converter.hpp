// escape_codes_basic_converter.hpp

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

#ifndef __YGGR_CHARSET_ESCAPE_CODES_BASIC_CONVERTER_HPP__
#define __YGGR_CHARSET_ESCAPE_CODES_BASIC_CONVERTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/math/odd_even_number.hpp>
#include <yggr/iterator_ex/iterator.hpp>
#include <yggr/func/is_callable.hpp>

#include <yggr/charset/convertible_traits.hpp>
#include <yggr/charset/hex_basic_converter.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/add_const.hpp>

namespace yggr
{
namespace charset
{

class escape_codes_basic_converter
{
public:
	YGGR_STATIC_CONSTANT(u32, E_Mode_Low = hex_converter_config::E_Mode_Low);
	YGGR_STATIC_CONSTANT(u32, E_Mode_Upper = hex_converter_config::E_Mode_Upper);
	YGGR_STATIC_CONSTANT(u32, E_Mode_Default = hex_converter_config::E_Mode_Default);

public:
	template<typename Char> YGGR_CONSTEXPR_OR_INLINE 
	static Char SEPUrl(void)
	{
		typedef Char now_char_type;
		return static_cast<now_char_type>('%');
	}

	template<typename Char> YGGR_CONSTEXPR_OR_INLINE 
	static Char SEPStringLiteral(void)
	{
		typedef Char now_char_type;
		return static_cast<now_char_type>('$');
	}

private:
	typedef escape_codes_basic_converter this_type;

public:
	// s_escape_byte
	template<typename Char> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_const<Char> >,
				is_convertible_char<Char>
			>,
			Char*
		>::type
		s_escape_byte(Char* pstr, u8 val, Char sep, u32 mode = E_Mode_Default)
	{
		typedef Char char_type;
		assert(pstr);

		if(pstr)
		{
			pstr[0] = sep;
			pstr[1] = static_cast<char_type>(hex_converter_config::s_hex_to_char_table(mode)[(val >> 4)]);
			pstr[2] = static_cast<char_type>(hex_converter_config::s_hex_to_char_table(mode)[val & u8(0x0f)]);
		}
		return pstr;
	}

	template<typename Char>
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_const<Char> >,
				is_convertible_char<Char>
			>,
			Char*
		>::type
		s_escape_bytes(Char* pstr, const u8* pdata, size_type size, Char sep, u32 mode = E_Mode_Default)
	{
		typedef Char char_type;

		assert((pstr && pdata && (pdata != reinterpret_cast<const u8*>(pstr))));

		if(!(pstr && pdata && (pdata != reinterpret_cast<const u8*>(pstr))))
		{
			return static_cast<char_type*>(0);
		}
		else if(!size)
		{
			return pstr;
		}
		else
		{
			char_type* pbuf = pstr;
			for(const u8 *i = pdata, *isize = pdata + size;  i != isize; ++i, pbuf += 3)
			{
				this_type::s_escape_byte<char_type>(pbuf, *i, sep, mode);
			}
			return pstr;
		}
	}

	// s_unescape_byte
	template<typename Char> inline
	static u8 s_unescape_byte(const Char* pstr)
	{
		assert(pstr);

		return 
			(hex_basic_converter::s_char_to_hex(pstr[1]) << 4)
			+ hex_basic_converter::s_char_to_hex(pstr[2]);
	}

	template<typename Char>
	static std::size_t s_unescape_bytes(u8* pdata, const Char* pstr, std::size_t size, Char sep)
	{
		typedef Char char_type;
		assert(pdata && pstr && size);

		if(!(pdata && pstr && size))
		{
			return 0;
		}
		else
		{
			const char_type* str_first = pstr;
			const char_type* str_last = iterator_ex::advance_copy(str_first, size);
			u8* pbuf = pdata;
			std::size_t now_byte_size = 0;

			for(const char_type* i = std::find(str_first, str_last, sep); i != str_last; i = std::find(str_first, str_last, sep))
			{
				if(iterator_ex::distance(i, str_last) < 3)
				{
					return 0;
				}

				now_byte_size = iterator_ex::distance(str_first, i) * sizeof(char_type);
				memcpy(pbuf, str_first, now_byte_size);
				iterator_ex::advance(pbuf, now_byte_size);
				*pbuf = this_type::s_unescape_byte(i);
				iterator_ex::advance(pbuf, 1);
				str_first = iterator_ex::advance_copy(i, 3);
			}

			if(str_first != str_last)
			{
				now_byte_size = iterator_ex::distance(str_first, str_last) * sizeof(char_type);
				memcpy(pbuf, str_first, now_byte_size);
				iterator_ex::advance(pbuf, now_byte_size);
			}

			return iterator_ex::distance(pdata, pbuf);
		}
	}
};

} // namespace charset
} // namespace yggr

#endif // __YGGR_CHARSET_ESCAPE_CODES_BASIC_CONVERTER_HPP__