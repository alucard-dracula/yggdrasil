//bits_to_types.hpp

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

#ifndef __YGGR_TYPE_TRAITS_BITS_TO_TYPES_HPP__
#define __YGGR_TYPE_TRAITS_BITS_TO_TYPES_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/null_t.hpp>

namespace yggr
{
namespace detail
{

template<std::size_t BitSize>
struct bits_to_signed_detail
	: public mplex::null_type
{
};

template<std::size_t BitSize>
struct bits_to_unsigned_detail
	: public mplex::null_type
{
};

template<std::size_t BitSize>
struct bits_to_float_detail
	: public mplex::null_type
{
};

} // namespace detail
} // namespace yggr


#define YGGR_PP_BITS_TO_SIGNED_DEF( __bits_size__, __type__ ) \
	namespace yggr { namespace detail { \
	template<> struct bits_to_signed_detail< __bits_size__ >{ typedef __type__ type; }; }}

#define YGGR_PP_BITS_TO_UNSIGNED_DEF( __bits_size__, __type__ ) \
	namespace yggr { namespace detail { \
	template<> struct bits_to_unsigned_detail< __bits_size__ >{ typedef __type__ type; }; }}

#define YGGR_PP_BITS_TO_FLOAT_DEF( __bits_size__, __type__ ) \
	namespace yggr { namespace detail { \
	template<> struct bits_to_float_detail< __bits_size__ >{ typedef __type__ type; }; }}


YGGR_PP_BITS_TO_SIGNED_DEF((1 << 3), s8)
YGGR_PP_BITS_TO_SIGNED_DEF((2 << 3), s16)
YGGR_PP_BITS_TO_SIGNED_DEF((4 << 3), s32)
YGGR_PP_BITS_TO_SIGNED_DEF((8 << 3), s64)

YGGR_PP_BITS_TO_UNSIGNED_DEF((1 << 3), u8)
YGGR_PP_BITS_TO_UNSIGNED_DEF((2 << 3), u16)
YGGR_PP_BITS_TO_UNSIGNED_DEF((4 << 3), u32)
YGGR_PP_BITS_TO_UNSIGNED_DEF((8 << 3), u64)

YGGR_PP_BITS_TO_FLOAT_DEF((4 << 3), f32)
YGGR_PP_BITS_TO_FLOAT_DEF((8 << 3), f64)


namespace yggr
{

template<std::size_t BitSize>
struct bits_to_signed
	: public detail::bits_to_signed_detail<BitSize>
{
};

template<std::size_t BitSize>
struct bits_to_unsigned
	: public detail::bits_to_unsigned_detail<BitSize>
{
};


template<std::size_t BitSize>
struct bits_to_float
	: public detail::bits_to_float_detail<BitSize>
{
};

} // namespace yggr


#endif //__YGGR_TYPE_TRAITS_BITS_TO_TYPES_HPP__