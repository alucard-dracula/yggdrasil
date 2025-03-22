//hex_basic_converter.cpp

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

#include <yggr/charset/hex_basic_converter.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

namespace yggr
{
namespace charset
{

/*static*/ 
u8* hex_basic_converter::s_string_to_hex(u8* phex, const char* pstr, size_type size)
{
	assert((phex && pstr && (phex != reinterpret_cast<const u8*>(pstr)) && math::is_even_number(size)));

	if(!(phex && pstr && (phex != reinterpret_cast<const u8*>(pstr)) && math::is_even_number(size)))
	{
		return 0;
	}
	else if(!size)
	{
		return phex;
	}
	else
	{
		u8 tmp1 = 0, tmp2 = 0;
		u8* j = phex;
		for(const char *i = pstr, *isize = pstr + size; i != isize; i += 2, ++j)
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

/*static*/ 
char* hex_basic_converter::s_hex_to_string(char* pstr, const u8* phex, size_type size, u32 mode)
{
	assert((pstr && phex && reinterpret_cast<u8*>(pstr) != phex));

	if(!(pstr && phex && reinterpret_cast<u8*>(pstr) != phex))
	{
		return 0;
	}
	else if(!size)
	{
		return pstr;
	}
	else
	{
		char* j = pstr;
		for(const u8 *i = phex, *isize = phex + size; i != isize; ++i, j += 2)
		{
			this_type::s_hex_to_char(j, *i, mode);
		}

		return pstr;
	}
}

} // namespace charset
} // namespace yggr
