//yggrdef.h

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

/*
*****************************************************
*	I am the bone of my codes						*
*	Zero is my body, and one is my blood.			*
*	I have created over a thousand sources.			*
*	Unknown to life, nor known to death.			*
*	Have withstood pain to create many algorithm	*
*	Yet, those hands will never hold anything.		*
*	So as I pray, Unlimited Binary Works.			*
*****************************************************
*/

#ifndef __YGGR_YGGR_DEF_H__
#define __YGGR_YGGR_DEF_H__

// disable auto create impl_lib
#if defined(_MSC_VER)
#	if !defined(YGGR_DLL_DEF)

#		if defined(BOOST_SYMBOL_EXPORT)
#			undef BOOST_SYMBOL_EXPORT
#		endif // BOOST_SYMBOL_EXPORT

#		if defined(BOOST_SYMBOL_IMPORT)
#			undef BOOST_SYMBOL_IMPORT
#		endif // BOOST_SYMBOL_IMPORT

#	define BOOST_SYMBOL_EXPORT
#	define BOOST_SYMBOL_IMPORT __declspec(dllimport)

#	endif // defined(YGGR_DLL_DEF)
#endif //#if defined(_MSC_VER)

#include <yggr/base/yggr_config.h>
#include <yggr/base/pragma_message.hpp>
#include <yggr/base/third_party_issues_config.hpp>
#include <yggr/base/new.hpp>

#include <boost/cstdint.hpp>

#include <cstddef> //char16_t, char32_t

#if (defined _MSC_VER) && (_MSC_VER < 1300) && (!defined(YGGR_NO_CWCHAR))
	typedef unsigned short wchar_t;
#endif // (defined _MSC_VER) && (_MSC_VER < 1300)

namespace yggr
{

typedef boost::int8_t s8; // signed 8-bit integer type
typedef boost::uint8_t u8; // unsigned 8-bit integer type

#if !defined(YGGR_NO_CHAR8_T)
	typedef char8_t c8;
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	typedef char16_t c16;
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	typedef char32_t c32;
#endif // YGGR_NO_CHAR32_T

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

// enum_value_t
namespace detail
{

enum enum_sample
{
	__E_SAMPLE__
};

template<std::size_t size>
struct enum_value_t_maker
{
	typedef u32 type;
};

template<>
struct enum_value_t_maker<1>
{
	typedef u8 type;
};

template<>
struct enum_value_t_maker<2>
{
	typedef u16 type;
};

template<>
struct enum_value_t_maker<4>
{
	typedef u32 type;
};

template<>
struct enum_value_t_maker<8>
{
	typedef u64 type;
};

} // namespace detail

typedef 
	detail::enum_value_t_maker
	<
		sizeof(detail::enum_sample)
	>::type enum_value_t;

} // namespace yggr

#endif //__YGGR_YGGR_DEF_H__
