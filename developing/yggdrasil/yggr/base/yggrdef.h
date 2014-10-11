//yggrdef.h

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

#ifndef __YGGR_DEF_H__
#define __YGGR_DEF_H__

#include <boost/cstdint.hpp>

/*
Compile Maroc info

*		Maroc								*Info

YGGR_SYSTEM_64							compile to 64bit system
YGGR_SERIALIZE_SIZE32					serialize size_type use 32bit
BOOST_HAS_ICU							regex use ICU fix utf8 string if undef it, regular_parse can not fix utf8 string
YGGR_AT_LINUX							linux platform
YGGR_AT_MAC                             mac platform
YGGR_INCLUDE_SEH_DEBUG					include seh model debuging
YGGR_USE_SEH							use seh model

YGGR_SYS_DEFAULT_STRING_CHARSET			now system string defalut charset, if undef it default "GBK"
YGGR_SYS_DEFAULT_WSTRING_CHARSET		now system string defalut charset, if undef it default "WCHAR_T"
YGGR_USE_STL_STRING						yggr::string use std::string, warning: segment(boost::interporcess) must use boost::container::basic_string
YGGR_USE_BOOST_STRING					yggr::string use boost::container::string (default)
YGGR_USE_FAST_MATH						yggr use fast math foo

YGGR_USE_CXX11							yggr use c++11

*/

enum
{
	E__pointer_byte_size__ = sizeof(int*)
};
//static const int __pointer_byte_size__ = sizeof(int*);
#ifndef YGGR_SYSTEM_64
#	if (E__pointer_byte_size__ == 8)
#		define YGGR_SYSTEM_64
#	endif // (E__pointer_byte_size__ == 8)
#endif // YGGR_SYSTEM_64

namespace yggr
{

typedef boost::int8_t s8; // signed 8-bit integer type
typedef boost::uint8_t u8; // unsigned 8-bit integer type

#if (defined _MSC_VER) && (_MSC_VER < 1300)
	typedef unsigned short wchar_t;
#endif // (defined _MSC_VER) && (_MSC_VER < 1300)

typedef boost::int16_t s16; // signed 16-bit integer type
typedef boost::uint16_t u16; // unsigned 16-bit integer type

typedef signed int sint; // int type;
typedef unsigned int uint; // uint type

typedef boost::int32_t s32; // signed 32-bit integer type
typedef boost::uint32_t u32; // unsigned 32-bit integer type

typedef boost::int64_t s64; // signed 64-bit integer type
typedef boost::uint64_t u64; // unsigned 64-bit integer type

typedef float f32; //32-bit float type

typedef double f64; //64-bit float type

#ifndef YGGR_SYSTEM_64
	typedef u32 size_type;
	typedef s32 s_size_type;

	typedef u32 ptr_num_type;
#else
	typedef u64 size_type;
	typedef s64 s_size_type;

	typedef u64 ptr_num_type;
#endif //YGGR_SYSTEM_64

#ifndef YGGR_SERIALIZE_SIZE32
	typedef size_type ser_size_type;
#else
	typedef u32 ser_size_type;
#endif //YGGR_SERIALIZE_SIZE32

} // namespace yggr

#endif //__YGGR_DEF_H__
