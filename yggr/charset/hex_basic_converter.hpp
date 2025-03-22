//hex_basic_converter.hpp

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

#ifndef __YGGR_CHARSET_HEX_BASIC_CONVERTER_HPP__
#define __YGGR_CHARSET_HEX_BASIC_CONVERTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/math/odd_even_number.hpp>

#include <yggr/charset/convertible_traits.hpp>
#include <yggr/charset/hex_converter_config.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/add_const.hpp>

#include <cassert>

namespace yggr
{
namespace charset
{

class hex_basic_converter
{
public:
	YGGR_STATIC_CONSTANT(u32, E_Mode_Low = hex_converter_config::E_Mode_Low);
	YGGR_STATIC_CONSTANT(u32, E_Mode_Upper = hex_converter_config::E_Mode_Upper);
	YGGR_STATIC_CONSTANT(u32, E_Mode_Default = hex_converter_config::E_Mode_Default);

private:
	typedef hex_basic_converter this_type;

public:
	// s_char_to_hex

	inline static u8 s_char_to_hex(char val)
	{
		switch(val)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return static_cast<u8>(val - '0');
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			return static_cast<u8>(10 + (val - 'A'));
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			return static_cast<u8>(10 + (val - 'a'));
		default:
			return u8(-1);
		}
	}

	template<typename Char> inline
	static
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			u8
		>::type
		s_char_to_hex(Char ch_val)
	{
		typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) char_type;
		typedef typename boost::add_const<char_type>::type const_char_type;

		char_type val = static_cast<char_type>(ch_val);
		switch(val)
		{
		case static_cast<const_char_type>('0'):
		case static_cast<const_char_type>('1'):
		case static_cast<const_char_type>('2'):
		case static_cast<const_char_type>('3'):
		case static_cast<const_char_type>('4'):
		case static_cast<const_char_type>('5'):
		case static_cast<const_char_type>('6'):
		case static_cast<const_char_type>('7'):
		case static_cast<const_char_type>('8'):
		case static_cast<const_char_type>('9'):
			return static_cast<u8>(val - static_cast<const_char_type>('0'));
		case static_cast<const_char_type>('A'):
		case static_cast<const_char_type>('B'):
		case static_cast<const_char_type>('C'):
		case static_cast<const_char_type>('D'):
		case static_cast<const_char_type>('E'):
		case static_cast<const_char_type>('F'):
			return static_cast<u8>(10 + (val - static_cast<const_char_type>('A')));
		case static_cast<const_char_type>('a'):
		case static_cast<const_char_type>('b'):
		case static_cast<const_char_type>('c'):
		case static_cast<const_char_type>('d'):
		case static_cast<const_char_type>('e'):
		case static_cast<const_char_type>('f'):
			return static_cast<u8>(10 + (val - static_cast<const_char_type>('a')));
		default:
			return u8(-1);
		}
	}

	// s_hex_to_char
	inline static char* s_hex_to_char(char* pstr, u8 val, u32 mode = E_Mode_Default)
	{
		assert(pstr);

		if(pstr)
		{
			pstr[0] = hex_converter_config::s_hex_to_char_table(mode)[(val >> 4)];
			pstr[1] = hex_converter_config::s_hex_to_char_table(mode)[val & u8(0x0f)];
		}
		return pstr;
	}

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
		s_hex_to_char(Char* pstr, u8 val, u32 mode = E_Mode_Default)
	{
		typedef Char char_type;
		assert(pstr);

		if(pstr)
		{
			pstr[0] = static_cast<char_type>(hex_converter_config::s_hex_to_char_table(mode)[(val >> 4)]);
			pstr[1] = static_cast<char_type>(hex_converter_config::s_hex_to_char_table(mode)[val & u8(0x0f)]);
		}
		return pstr;
	}

	// s_string_to_hex
	static u8* s_string_to_hex(u8* phex, const char* pstr, size_type size);

	template<typename Char>
	static
	typename
		boost::enable_if
		<
			is_convertible_char<Char>,
			u8*
		>::type
		s_string_to_hex(u8* phex, const Char* pstr, size_type size)
	{
		typedef Char char_type;

		assert((phex && pstr && (phex != reinterpret_cast<const u8*>(pstr)) && math::is_even_number(size)));

		if(!(phex && pstr && (phex != reinterpret_cast<const u8*>(pstr)) && math::is_even_number(size)))
		{
			return static_cast<u8*>(0);
		}
		else if(!size)
		{
			return phex;
		}
		else
		{
			u8 tmp1 = 0, tmp2 = 0;
			u8* j = phex;
			for(const char_type *i = pstr, *isize = pstr + size; i != isize; i += 2, ++j)
			{
				tmp1 = this_type::s_char_to_hex(i[0]);
				if(tmp1 & 0xf0)
				{
					return 0;
				}

				tmp2 = this_type::s_char_to_hex(i[1]);
				if(tmp2 & 0xf0)
				{
					return 0;
				}
		
				*j = ((tmp1 << 4) | tmp2);
			}
			return phex;
		}
	}

	// s_hex_to_string
	static char* s_hex_to_string(char* pstr, const u8* phex, size_type size, u32 mode = E_Mode_Default);

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
		s_hex_to_string(Char* pstr, const u8* phex, size_type size, u32 mode = E_Mode_Default)
	{
		typedef Char char_type;

		assert((pstr && phex && reinterpret_cast<u8*>(pstr) != phex));

		if(!(pstr && phex && reinterpret_cast<u8*>(pstr) != phex))
		{
			return static_cast<char_type*>(0);
		}
		else if(!size)
		{
			return pstr;
		}
		else
		{
			char_type* j = pstr;
			for(const u8 *i = phex, *isize = phex + size; i != isize; ++i, j += 2)
			{
				this_type::s_hex_to_char<char_type>(j, *i, mode);
			}

			return pstr;
		}
	}
};

} // namespace charset
} // namespace yggr

#endif // __YGGR_CHARSET_HEX_BASIC_CONVERTER_HPP__