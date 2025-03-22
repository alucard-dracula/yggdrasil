//utf8_foo.cpp

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

#include <yggr/charset/utf8_foo.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <boost/mpl/bool.hpp>

#include <cassert>
#include <cstring>
#include <memory>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER


namespace yggr
{
namespace charset
{

//-------------------utf8_char-------------------------

//-------------utf8_string--------------------------------------------

/*static*/
size_type utf8_foo::strlen(const char* str, size_type n)
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(0 < static_cast<size_type>(-1))>));

	assert(str);

	if(!str || n == 0)
	{
		return 0;
	}

	size_type i = 0, count = 0;
	char c = 0;

	for(;i < n;)
	{
		c = str[i];
		if(c == 0)
		{
			if(n == static_cast<size_type>(-1))
			{
				break;
			}
			else
			{
				++count;
				++i;
				continue;
			}
		}
		else if((c & 0x80) == 0x00)
		{
			++count;
			++i;
			continue;
		}
// thanks for the reminder given by yejianfeiVictor(QQ1372116815)
#	ifdef YGGR_USE_UTF8_EX_MODE
		else if((c & 0xfe) == 0xfc)
		{
			if((i + 5 < n)
				&& ((str[i + 1] & 0xc0) == 0x80)
					&& ((str[i + 2] & 0xc0) == 0x80)
					&& ((str[i + 3] & 0xc0) == 0x80)
					&& ((str[i + 4] & 0xc0) == 0x80)
					&& ((str[i + 5] & 0xc0) == 0x80))
			{
				++count;
				i += 6;
				continue;
			}
			else
			{
				return 0;
			}
		}
		else if((c & 0xfc) == 0xf8)
		{
			if((i + 4 < n)
				&& ((str[i + 1] & 0xc0) == 0x80)
					&& ((str[i + 2] & 0xc0) == 0x80)
					&& ((str[i + 3] & 0xc0) == 0x80)
					&& ((str[i + 4] & 0xc0) == 0x80))
			{
				++count;
				i += 5;
				continue;
			}
			else
			{
				return 0;
			}
		}
#	endif //YGGR_USE_UTF8_EX_MODE
		else if((c & 0xf8) == 0xf0)
		{
			if((i + 3 < n)
				&& ((str[i + 1] & 0xc0) == 0x80)
					&& ((str[i + 2] & 0xc0) == 0x80)
					&& ((str[i + 3] & 0xc0) == 0x80))
			{
				++count;
				i += 4;
				continue;
			}
			else
			{
				return 0;
			}
		}
		else if((c & 0xf0) == 0xe0)
		{
			if((i + 2 < n)
				&& ((str[i + 1] & 0xc0) == 0x80)
					&& ((str[i + 2] & 0xc0) == 0x80))
			{
				++count;
				i += 3;
				continue;
			}
			else
			{
				return 0;
			}
		}
		else if((c & 0xe0) == 0xc0)
		{
			if((i + 1 < n) && ((str[i + 1] & 0xc0) == 0x80) )
			{
				++count;
				i += 2;
				continue;
			}
			else
			{
				return 0;
			}
		}
		else 
			return 0;
	}

	return 
		i < n?
			(str[i] == 0? count : 0)
			: (i == n? count : 0);
}

/*static*/
std::pair<size_type, size_type> utf8_foo::strlen_both(const char* str, size_type n)
{
	typedef std::pair<size_type, size_type> ret_type;

	BOOST_MPL_ASSERT((boost::mpl::bool_<(0 < static_cast<size_type>(-1))>));

	assert(str);

	if(!str || n == 0)
	{
		return ret_type(0, 0);
	}

	size_type i = 0, count = 0;
	char c = 0;

	for(;i < n;)
	{
		c = str[i];
		if(c == 0)
		{
			if(n == static_cast<size_type>(-1))
			{
				break;
			}
			else
			{
				++count;
				++i;
				continue;
			}
		}
		else if((c & 0x80) == 0x00)
		{
			++count;
			++i;
			continue;
		}
// thanks for the reminder given by yejianfeiVictor(QQ1372116815)
#	ifdef YGGR_USE_UTF8_EX_MODE
		else if((c & 0xfe) == 0xfc)
		{
			if((i + 5 < n)
				&& ((str[i + 1] & 0xc0) == 0x80)
					&& ((str[i + 2] & 0xc0) == 0x80)
					&& ((str[i + 3] & 0xc0) == 0x80)
					&& ((str[i + 4] & 0xc0) == 0x80)
					&& ((str[i + 5] & 0xc0) == 0x80))
			{
				++count;
				i += 6;
				continue;
			}
			else
			{
				return ret_type(0, i + charset_base_foo::strnlen(&str[i], n - i));
			}
		}
		else if((c & 0xfc) == 0xf8)
		{
			if((i + 4 < n)
				&& ((str[i + 1] & 0xc0) == 0x80)
					&& ((str[i + 2] & 0xc0) == 0x80)
					&& ((str[i + 3] & 0xc0) == 0x80)
					&& ((str[i + 4] & 0xc0) == 0x80))
			{
				++count;
				i += 5;
				continue;
			}
			else
			{
				return ret_type(0, i + charset_base_foo::strnlen(&str[i], n - i));
			}
		}
#	endif //YGGR_USE_UTF8_EX_MODE
		else if((c & 0xf8) == 0xf0)
		{
			if((i + 3 < n)
				&& ((str[i + 1] & 0xc0) == 0x80)
					&& ((str[i + 2] & 0xc0) == 0x80)
					&& ((str[i + 3] & 0xc0) == 0x80))
			{
				++count;
				i += 4;
				continue;
			}
			else
			{
				return ret_type(0, i + charset_base_foo::strnlen(&str[i], n - i));
			}
		}
		else if((c & 0xf0) == 0xe0)
		{
			if((i + 2 < n)
				&& ((str[i + 1] & 0xc0) == 0x80)
					&& ((str[i + 2] & 0xc0) == 0x80))
			{
				++count;
				i += 3;
				continue;
			}
			else
			{
				return ret_type(0, i + charset_base_foo::strnlen(&str[i], n - i));
			}
		}
		else if((c & 0xe0) == 0xc0)
		{
			if((i + 1 < n) && ((str[i + 1] & 0xc0) == 0x80) )
			{
				++count;
				i += 2;
				continue;
			}
			else
			{
				return ret_type(0, i + charset_base_foo::strnlen(&str[i], n - i));
			}
		}
		else 
			return ret_type(0, i + charset_base_foo::strnlen(&str[i], n - i));
	}

	return 
		i < n?
			(str[i] == 0? ret_type(count, i) : ret_type(0, i))
			: (i == n? ret_type(count, i) : ret_type(0, i));
}

/*static*/
size_type utf8_foo::strnlen(const char* str, size_type n)
{
	if(!str || n == 0)
	{
		return 0;
	}

	size_type i = 0, count = 0;
	char c = 0;

	for(;i < n;)
	{
		c = str[i];
		if(c == 0)
		{
			if(n == static_cast<size_type>(-1))
			{
				break;
			}
			else
			{
				++count;
				++i;
				continue;
			}
		}
		else if((c & 0x80) == 0x00)
		{
			++count;
			++i;
			continue;
		}
// thanks for the reminder given by yejianfeiVictor(QQ1372116815)
#	ifdef YGGR_USE_UTF8_EX_MODE
		else if((c & 0xfe) == 0xfc)
		{
			if((i + 5 < n)
				&& ((str[i + 1] & 0xc0) == 0x80)
					&& ((str[i + 2] & 0xc0) == 0x80)
					&& ((str[i + 3] & 0xc0) == 0x80)
					&& ((str[i + 4] & 0xc0) == 0x80)
					&& ((str[i + 5] & 0xc0) == 0x80))
			{
				++count;
				i += 6;
				continue;
			}
			else
			{
				return count;
			}
		}
		else if((c & 0xfc) == 0xf8)
		{
			if((i + 4 < n)
				&& ((str[i + 1] & 0xc0) == 0x80)
					&& ((str[i + 2] & 0xc0) == 0x80)
					&& ((str[i + 3] & 0xc0) == 0x80)
					&& ((str[i + 4] & 0xc0) == 0x80))
			{
				++count;
				i += 5;
				continue;
			}
			else
			{
				return count;
			}
		}
#	endif //YGGR_USE_UTF8_EX_MODE
		else if((c & 0xf8) == 0xf0)
		{
			if((i + 3 < n)
				&& ((str[i + 1] & 0xc0) == 0x80)
					&& ((str[i + 2] & 0xc0) == 0x80)
					&& ((str[i + 3] & 0xc0) == 0x80))
			{
				++count;
				i += 4;
				continue;
			}
			else
			{
				return count;
			}
		}
		else if((c & 0xf0) == 0xe0)
		{
			if((i + 2 < n)
				&& ((str[i + 1] & 0xc0) == 0x80)
					&& ((str[i + 2] & 0xc0) == 0x80))
			{
				++count;
				i += 3;
				continue;
			}
			else
			{
				return count;
			}
		}
		else if((c & 0xe0) == 0xc0)
		{
			if((i + 1 < n) && ((str[i + 1] & 0xc0) == 0x80) )
			{
				++count;
				i += 2;
				continue;
			}
			else
			{
				return count;
			}
		}
		else
			return count;
	}

	return count;
}

/*static*/
std::pair<size_type, size_type> utf8_foo::strnlen_both(const char* str, size_type n)
{
	typedef std::pair<size_type, size_type> ret_type;

	if(!str || n == 0)
	{
		return ret_type(0, 0);
	}

	size_type i = 0, count = 0;
	char c = 0;

	for(;i < n;)
	{
		c = str[i];
		if(c == 0)
		{
			if(n == static_cast<size_type>(-1))
			{
				break;
			}
			else
			{
				++count;
				++i;
				continue;
			}
		}
		else if((c & 0x80) == 0x00)
		{
			++count;
			++i;
			continue;
		}
// thanks for the reminder given by yejianfeiVictor(QQ1372116815)
#	ifdef YGGR_USE_UTF8_EX_MODE
		else if((c & 0xfe) == 0xfc)
		{
			if((i + 5 < n)
				&& ((str[i + 1] & 0xc0) == 0x80)
					&& ((str[i + 2] & 0xc0) == 0x80)
					&& ((str[i + 3] & 0xc0) == 0x80)
					&& ((str[i + 4] & 0xc0) == 0x80)
					&& ((str[i + 5] & 0xc0) == 0x80))
			{
				++count;
				i += 6;
				continue;
			}
			else
			{
				return ret_type(count, i);
			}
		}
		else if((c & 0xfc) == 0xf8)
		{
			if((i + 4 < n)
				&& ((str[i + 1] & 0xc0) == 0x80)
					&& ((str[i + 2] & 0xc0) == 0x80)
					&& ((str[i + 3] & 0xc0) == 0x80)
					&& ((str[i + 4] & 0xc0) == 0x80))
			{
				++count;
				i += 5;
				continue;
			}
			else
			{
				return ret_type(count, i);
			}
		}
#	endif //YGGR_USE_UTF8_EX_MODE
		else if((c & 0xf8) == 0xf0)
		{
			if((i + 3 < n)
				&& ((str[i + 1] & 0xc0) == 0x80)
					&& ((str[i + 2] & 0xc0) == 0x80)
					&& ((str[i + 3] & 0xc0) == 0x80))
			{
				++count;
				i += 4;
				continue;
			}
			else
			{
				return ret_type(count, i);
			}
		}
		else if((c & 0xf0) == 0xe0)
		{
			if((i + 2 < n)
				&& ((str[i + 1] & 0xc0) == 0x80)
					&& ((str[i + 2] & 0xc0) == 0x80))
			{
				++count;
				i += 3;
				continue;
			}
			else
			{
				return ret_type(count, i);
			}
		}
		else if((c & 0xe0) == 0xc0)
		{
			if((i + 1 < n) && ((str[i + 1] & 0xc0) == 0x80) )
			{
				++count;
				i += 2;
				continue;
			}
			else
			{
				return ret_type(count, i);
			}
		}
		else
			return ret_type(count, i);
	}

	return ret_type(count, i);
}

// utf8_len_to_byte_size
/*static*/ 
size_type utf8_foo::utf8_length_to_byte_size(const char* str, size_type utf8_len)
{
	assert(str);
	if(!(str && utf8_len))
	{
		return 0;
	}
	else if(utf8_len == size_type(-1))
	{
		return std::strlen(str);
	}
	else
	{
		size_type byte_size = 0;

		for(size_type i = 0; (i != utf8_len) && (this_type::char_to_number(str + byte_size)); ++i)
		{
			byte_size += this_type::charlen(str + byte_size);
		}

		return byte_size;
	}
}

// find

/*static*/ 
size_type utf8_foo::find(const char* i1, const char* i2, const char* cmp)
{
	assert((i1 && i2 && cmp));
	assert((i1 <= i2));

	size_type pos = 0;
	for(; i1 != i2; i1 += this_type::charlen(i1), ++pos)
	{
		if(this_type::charcmp_eq(i1, cmp))
		{
			return pos;
		}
	}

	return static_cast<size_type>(-1);
}

//search
/*static*/ 
size_type utf8_foo::search(const char* i1, const char* i2, 
							const char* cmp_i1, const char* cmp_i2)
{
	assert((i1 && i2 && cmp_i1 && cmp_i2));
	assert((i1 <= i2 && cmp_i1 <= cmp_i2));

	size_type src_len = std::distance(i1, i2);
	size_type cmp_len = std::distance(cmp_i1, cmp_i2);

	if(cmp_i1 == cmp_i2)
	{
		return 0;
	}
	else if(src_len < cmp_len)
	{
		return static_cast<size_type>(-1);
	}
	else
	{
		size_type pos = 0;
		for(; cmp_len <= static_cast<size_type>(std::distance(i1, i2)); i1 += this_type::charlen(i1), ++pos)
		{
			assert(i1 <= i2);
			// in the release version of the direct use of "for" in 100 million test under, 
			// fast about 0.2 seconds
			if(0 == std::memcmp(i1, cmp_i1, cmp_len))
			{
				return pos;
			}
		}

		return static_cast<size_type>(-1);
	}
}

/*static*/ 
size_type utf8_foo::search_n(const char* i1, const char* i2, 
								size_type count, const char* cmp)
{
	assert(i1 && i2 && i1 <= i2 && cmp);
	size_type cmp_step = this_type::charlen(cmp);

	if(!count)
	{
		return 0;
	}
	else if(static_cast<size_type>(std::distance(i1, i2)) < count * cmp_step)
	{
		return static_cast<size_type>(-1);
	}
	else
	{
		size_type i = 0;
		size_type ret_index = 0;
		
		while(i1 != i2 && i != count)
		{
			assert(i1 <= i2);
			if(this_type::charcmp_eq(i1, cmp))
			{
				i1 += cmp_step;
				++i;
			}
			else
			{
				ret_index += (i + 1);
				i = 0;
				i1 += this_type::charlen(i1);
			}
		}

		return i == count? ret_index : static_cast<size_type>(-1);
	}
}

} // namespace charset
} // namespace yggr
