//bit_counter.hpp

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

#ifndef __YGGR_UTILITY_BIT_COUNTER_HPP__
#define __YGGR_UTILITY_BIT_COUNTER_HPP__

#include <yggr/base/yggrdef.h>
#include <cassert>

namespace yggr
{
namespace utility
{

template<bool b>
struct bit_counter;

template<>
struct bit_counter<true>
{
public:
	typedef bit_counter this_type;

public:
	template<typename T> inline 
	size_type operator()(const T& val) const
	{
		return this_type::bit_count(val);
	}

public:
	template<typename T>
	static size_type bit_count(const T& val)
	{
		typedef T value_type;
		const u8* pbyte = reinterpret_cast<const u8*>(&val);
		size_type s = 0;
		for(size_type i = 0, isize = sizeof(value_type); i != isize; ++i)
		{
			s += this_type::prv_s_bit_count_of_byte_size(pbyte[i]);
		}

		return s;
	}
private:
	inline static size_type prv_s_bit_count_of_byte_size(u8 t)
	{
		typedef struct byte_t
		{
			unsigned _0:1; 
			unsigned _1:1; 
			unsigned _2:1; 
			unsigned _3:1; 
			unsigned _4:1; 
			unsigned _5:1; 
			unsigned _6:1; 
			unsigned _7:1; 
		} byte_t_type;

		byte_t_type* pbyte = reinterpret_cast<byte_t_type*>(&t);
		assert(pbyte);
		return pbyte->_0 
				+ pbyte->_1 
				+ pbyte->_2 
				+ pbyte->_3 
				+ pbyte->_4
				+ pbyte->_5
				+ pbyte->_6
				+ pbyte->_7;
	}
};

template<>
struct bit_counter<false>
{
public:
	typedef bit_counter this_type;

public:
	template<typename T> inline
	size_type operator()(const T& val) const
	{
		return this_type::bit_count(val);
	}

public:
	template<typename T> inline
	static size_type bit_count(const T& val)
	{
		typedef T value_type;
		return sizeof(value_type) * 8 - bit_counter<true>::bit_count(val);
	}
};


} // namespace utility

using utility::bit_counter;

} // namespace yggr

#endif // __YGGR_UTILITY_BIT_COUNTER_HPP__