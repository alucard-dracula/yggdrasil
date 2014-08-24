//utf8_foo.cpp

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

#if defined(_MSC_VER) && defined(_DEBUG)
#	include <vld.h>
#endif // _MSC_VER

#include <yggr/charset/utf8_foo.hpp>
#include <cstring>

namespace yggr
{
namespace charset
{
//-------------------utf8_char-------------------------
size_type utf8_foo::charlen(char c)
{
	if((c & 0x80) == 0x00)
	{
		return 1;
	}

	return 0;
}

size_type utf8_foo::charlen(const char* first)
{
	if(!first)
	{
		return 0;
	}

	char c = *first;

	if((c & 0x80) == 0x00)
	{
		return 1;
	}

	if(((c & 0xf8) == 0xf0)
		&& (((first[1] & 0xc0) == 0x80)
			&& ((first[2] & 0xc0) == 0x80)
			&& ((first[3] & 0xc0) == 0x80)))
	{
		return 4;
	}

	if(((c & 0xf0) == 0xe0)
		&& (((first[1] & 0xc0) == 0x80)
			&& ((first[2] & 0xc0) == 0x80)))
	{
		return 3;
	}

	if(((c & 0xe0) == 0xc0)
		&& (((first[1] & 0xc0) == 0x80)))
	{
		return 2;
	}

	return 0;
}

size_type utf8_foo::charlen(const wchar_t* first)
{
	return this_type::charlen((const char*) first);
}

bool utf8_foo::is_only_char(char src)
{
	return this_type::charlen(src) == 1;
}

bool utf8_foo::is_only_char(const char* src)
{
	return this_type::strlen(src) == 1;
}

bool utf8_foo::is_only_char(const wchar_t* src)
{
	return this_type::strlen(src) == 1;
}

//-------------utf8_string--------------------------------------------

size_type utf8_foo::strlen(const char* str, size_type n)
{
	if(!str || n == 0)
	{
		return 0;
	}

	size_type count = 0;
	char c = 0;

	for(size_type i = 0, isize = (n == size_type(-1)? std::strlen(str) : n); i != isize;)
	{
		c = str[i];
		if((c & 0x80) == 0x00)
		{
			++count;
			++i;
			continue;
		}
		else
		{
			if((c & 0xf8) == 0xf0)
			{
				if((i + 3 < isize)
					&& ((str[i + 1] & 0xc0) == 0x80)
						&& ((str[i + 2] & 0xc0) == 0x80)
						&& ((str[i + 3] & 0xc0) == 0x80))
				{
					++count;
					i += 4;
					continue;
				}

				return 0;
			}

			if((c & 0xf0) == 0xe0)
			{
				if((i + 2 < isize)
					&& ((str[i + 1] & 0xc0) == 0x80)
						&& ((str[i + 2] & 0xc0) == 0x80))
				{
					++count;
					i += 3;
					continue;
				}
				return 0;
			}

			if((c & 0xe0) == 0xc0)
			{
				if((i + 1 < isize) && ((str[i + 1] & 0xc0) == 0x80) )
				{
					++count;
					i += 2;
					continue;
				}

				return 0;
			}
		}

		return 0;
	}

	return count;
}

size_type utf8_foo::strlen(const char* start, const char* last)
{
	assert(start <= last);
	return this_type::strlen(start, last - start);
}

size_type utf8_foo::strlen(const wchar_t* start, const wchar_t* last)
{
	assert(start <= last);
	return this_type::strlen((const char*)start, (last - start) * sizeof(wchar_t));
}

size_type utf8_foo::strlen(const wchar_t* str, size_type n)
{
	return this_type::strlen((const char*)str, n == size_type(-1)? n : n * sizeof(wchar_t));
}

bool utf8_foo::is_utf8_string(const char* str)
{
	return this_type::strlen(str) != 0;
}

bool utf8_foo::is_utf8_string(const wchar_t* str)
{
	return this_type::is_utf8_string((const char*) str);
}

const char* utf8_foo::get_charset_name(const char* str)
{
	return this_type::is_utf8_string(str)?
			IN_CHARSET_NAME
			: default_charset_config::get_default_string_charset_name<char>();
}

const char* utf8_foo::get_charset_name(const wchar_t* str)
{
	return this_type::is_utf8_string(str)?
			IN_CHARSET_NAME
			: default_charset_config::get_default_string_charset_name<wchar_t>();
}

} // namespace charset
} // namespace yggr
