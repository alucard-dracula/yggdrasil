//sysbit.hpp

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

#ifndef __YGGR_MPLEX_SYSBIT_HPP__
#define __YGGR_MPLEX_SYSBIT_HPP__

#include <yggr/base/static_constant.hpp>

namespace yggr
{
namespace mplex
{

template<std::size_t ByteSize>
struct sysbit
{
	typedef sysbit type;
	YGGR_STATIC_CONSTANT(std::size_t, value = (ByteSize << 3));
};

template<typename T>
struct sizeof_bit
	: public sysbit<sizeof(T)>
{
};

typedef sysbit<1> sysbit_8;
typedef sysbit<2> sysbit_16;
typedef sysbit<4> sysbit_32;
typedef sysbit<8> sysbit_64;
typedef sysbit<16> sysbit_128;
typedef sysbit<32> sysbit_256;

typedef sizeof_bit<void*>::type sysbit_now;
typedef sizeof_bit<double>::type sysbit_max;

} // namespace mplex
} // namespace yggr

#endif //__YGGR_MPLEX_SYSBIT_HPP__