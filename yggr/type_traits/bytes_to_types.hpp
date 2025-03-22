//bytes_to_types.hpp

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

#ifndef __YGGR_TYPE_TRAITS_BYTES_TO_TYPES_HPP__
#define __YGGR_TYPE_TRAITS_BYTES_TO_TYPES_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/bits_to_types.hpp>

namespace yggr
{

template<std::size_t ByteSize>
struct bytes_to_signed
	: public bits_to_signed<(ByteSize << 3)> // ByteSize * 8
{
};

template<std::size_t ByteSize>
struct bytes_to_unsigned
	: public bits_to_unsigned<(ByteSize << 3)> // ByteSize * 8
{
};


template<std::size_t ByteSize>
struct bytes_to_float
	: public bits_to_float<(ByteSize << 3)> // ByteSize * 8
{
};

} // namespace yggr


#endif //__YGGR_TYPE_TRAITS_BYTES_TO_TYPES_HPP__