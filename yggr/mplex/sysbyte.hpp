//sysbyte.hpp

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

#ifndef __YGGR_MPLEX_SYSBYTE_HPP__
#define __YGGR_MPLEX_SYSBYTE_HPP__

#include <yggr/base/static_constant.hpp>

namespace yggr
{
namespace mplex
{

template<std::size_t ByteSize>
struct sysbyte
{
	typedef sysbyte type;
	YGGR_STATIC_CONSTANT(std::size_t, value = ByteSize);
};

template<typename T>
struct sizeof_byte
	: public sysbyte<sizeof(T)>
{
};

typedef sysbyte<1> sysbyte_1;
typedef sysbyte<2> sysbyte_2;
typedef sysbyte<4> sysbyte_4;
typedef sysbyte<8> sysbyte_8;
typedef sysbyte<16> sysbyte_16;
typedef sysbyte<32> sysbyte_32;

typedef sizeof_byte<void*>::type sysbyte_now;
typedef sizeof_byte<double>::type sysbyte_max;

} // namespace mplex
} // namespace yggr

#endif //__YGGR_MPLEX_SYSBYTE_HPP__