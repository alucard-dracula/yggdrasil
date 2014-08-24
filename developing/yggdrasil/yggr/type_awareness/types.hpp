//types.hpp

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

#ifndef __YGGR_TYPE_AWARENESS_TYPES_HPP__
#define __YGGR_TYPE_AWARENESS_TYPES_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_awareness/typeids_def.hpp>
#include <yggr/container/string.hpp>
#include <yggr/container/utf8_string.hpp>

enum
	{
		E_TYPE_IDS = 0,
		E_CHAR,
		E_WCHAR,
		E_S8,
		E_U8,
		E_S16,
		E_U16,
		E_S32,
		E_U32,
		E_S64,
		E_U64,
		E_F32,
		E_F64,
		E_STL_STRING,
		E_STL_WSTRING,
		E_BOOST_STRING,
		E_BOOST_WSTRING,
		E_UTF8_STRING,

		E_compile_u32 = 0xffffffff
	};

#define YGGR_AWARNS_SET_TYPEIDS_TAB( __id__, __type__ ) \
	namespace yggr { namespace type_awareness { \
	template<> struct<__id__> {  \
		enum { value = __id__ }; \
		typedef __type__ type; }; } }


namespace yggr
{
namespace type_awareness
{

template<u32 tid>
struct id_to_type;

template<typename T>
struct type_to_id;

} // namespace type_awareness
} // namespace yggr

YGGR_AWARNS_SET_TYPEIDS_TAB(E_CHAR, char)
YGGR_AWARNS_SET_TYPEIDS_TAB(E_WCHAR, wchar_t)

YGGR_AWARNS_SET_TYPEIDS_TAB(E_S8, s8)
YGGR_AWARNS_SET_TYPEIDS_TAB(E_U8, u8)
YGGR_AWARNS_SET_TYPEIDS_TAB(E_S16, s16)
YGGR_AWARNS_SET_TYPEIDS_TAB(E_U16, u16)
YGGR_AWARNS_SET_TYPEIDS_TAB(E_S32, s32)
YGGR_AWARNS_SET_TYPEIDS_TAB(E_U32, u32)
YGGR_AWARNS_SET_TYPEIDS_TAB(E_S64, s64)
YGGR_AWARNS_SET_TYPEIDS_TAB(E_U64, u64)
YGGR_AWARNS_SET_TYPEIDS_TAB(E_F32, f32)
YGGR_AWARNS_SET_TYPEIDS_TAB(E_F64, f64)
YGGR_AWARNS_SET_TYPEIDS_TAB(E_STL_STRING, std::string)
YGGR_AWARNS_SET_TYPEIDS_TAB(E_STL_WSTRING, std::wstring)
YGGR_AWARNS_SET_TYPEIDS_TAB(E_BOOST_STRING, boost::container::string)
YGGR_AWARNS_SET_TYPEIDS_TAB(E_BOOST_WSTRING, boost::container::wstring)
YGGR_AWARNS_SET_TYPEIDS_TAB(E_UTF8_STRING, utf8_string)

#endif // __YGGR_TYPE_AWARENESS_TYPES_HPP__