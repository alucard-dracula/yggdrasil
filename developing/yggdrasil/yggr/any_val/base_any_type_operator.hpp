//base_any_type_operator.hpp

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

#ifndef __YGGR_ANY_VAL_BASE_ANY_TYPE_OPERATOR_HPP__
#define __YGGR_ANY_VAL_BASE_ANY_TYPE_OPERATOR_HPP__

#include <functional>
#include <cassert>
#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/any_val/any.hpp>
#include <yggr/any_val/any_set.hpp> // ANY_SET
#include <yggr/nonable/noncreateable.hpp>

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning( disable : 4018)
#	pragma warning( disable : 4244)
#	pragma warning( disable : 4800)
#	pragma warning( disable : 4804)
#	pragma warning( disable : 4805)
#	pragma warning (disable : 4624)
#endif //_MSC_VER


//-------------------------math_1---------------------------------------------
#define ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(__type__, __foo__, __any__) \
	if(__any__.type() == typeid(__type__)) { \
		const __type__ *p = yggr::any_cast<__type__>(&__any__); \
		return p? any_type(__foo__(*p)) : any_type(); }

#define ANY_MATH_RET_CHECK_CAL_1(__type__, __foo__, __any__) \
	if(__any__.type() == typeid(__type__)) { \
		__type__ *p = yggr::any_cast<__type__>(&__any__); \
		if(p){ __foo__(*p); } \
		return __any__; } 

#define ANY_MATH_CONST_RET_CHECK_CAL_1(__type__, __foo__, __any__) \
	if(__any__.type() == typeid(__type__)) { \
		__type__ *p = yggr::any_cast<__type__>(&__any__); \
		return p? any_type(__foo__(*p)) : any_type(); }

//------------------------------math_2----------------------------------------
#define ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(__type__, __any__) \
	if(__any__.type() == typeid(__type__)) { \
		const __type__ *p1 = yggr::any_cast<__type__>(&__any__); \
		if(!p1) { return __any__; } 

#define ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(__type__, __foo__, __any__) \
	if(__any__.type() == typeid(__type__)) { \
		const __type__ *p2 = yggr::any_cast<__type__>(&__any__); \
		if(!p2) { return any_type(*p1);} \
		return any_type(__foo__(*p1, *p2)); }

#define ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END } 

#define ANY_MATH_NRET_CHECK_CAL_2_START(__type__, __any__) \
	if(__any__.type() == typeid(__type__)) { \
		__type__ *p1 = yggr::any_cast<__type__>(&__any__); \
		if(!p1) { return; } 

#define ANY_MATH_NRET_CHECK_CAL_2(__type__, __foo__, __any__) \
	if(__any__.type() == typeid(__type__)) { \
		const __type__ *p2 = yggr::any_cast<__type__>(&__any__); \
		if(!p2) { return;} \
		__foo__(*p1, *p2); \
		return; }

#define ANY_MATH_NRET_CHECK_CAL_2_END } 

//----------------------------------logic-----------------------------

#define ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(__type__, __bool__, __any__) \
	if(__any__.type() == typeid(__type__)) { \
		const __type__ *p = yggr::any_cast<__type__>(&__any__); \
		if(!p) { return false;} \
		__bool__ = *p; }

//--------------------------------logic_2------------------------------

#define ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(__type__, __any__) \
	if(__any__.type() == typeid(__type__)) { \
		const __type__ *p1 = yggr::any_cast<__type__>(&__any__); \
		if(!p1) { return false; } 

#define ANY_LOGIC_RET_CONST_CHECK_CAL_2(__type__, __foo__, __any__) \
	if(__any__.type() == typeid(__type__)) { \
		const __type__ *p2 = yggr::any_cast<__type__>(&__any__); \
		if(!p2) { return false;} \
		return __foo__(*p1, *p2); }

#define ANY_LOGIC_RET_CONST_CHECK_CAL_2_END } 

//----------------------any_init-----------------------------------

//#define ANY_SET(__type__, __val__) static_cast<__type__>(__val__)


//-------------------------------------------------------------------------

namespace yggr
{
namespace any_val
{

class base_any_type_operator : private nonable::noncreateable
{
public:

	typedef yggr::any any_type;
	typedef base_any_type_operator this_type;
	
public:
	template<typename L, typename Handler>
	static const L math_op(const L& l, const Handler& handler)
	{
		return handler(l);
	}

	template<typename L, typename R, typename Handler>
	static const L math_op(const L& l, const R& r, const Handler& handler)
	{
		any_type ta(handler(l, r));
		return ta;
	}

	template<typename L, typename R, typename Handler>
	static void set_op(L& l, const R& r, const Handler& handler)
	{
		handler(l, r);
	}

	template<typename L, typename Handler>
	static bool logic_op(const L& l, const Handler& handler)
	{
		return handler(l);
	}

	template<typename L, typename R, typename Handler>
	static bool logic_op(const L& l, const R& r, const Handler& handler)
	{
		return handler(l, r);
	}

//public:
protected:
	template<typename L>
	static const L negate(const L& l)
	{
		assert(!this_type::is_any<L>());
		return -l;
	}

	static const any_type negate(const any_type& l)
	{
		if(l.empty())
		{
			return l;
		}
		
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(s8, negate, l);
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(s16, negate, l);
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(s32, negate, l);
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(s64, negate, l);
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(f32, negate, l);
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(sint, negate, l);
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(char, negate, l);

		return l;
	}

	template<typename L>
	static L& befor_plus_plus(L& l)
	{
		assert(!this_type::is_any<L>());
		return ++l;
	}

	static any_type& befor_plus_plus(any_type& l)
	{

		if(l.empty())
		{
			return l;
		}

		ANY_MATH_RET_CHECK_CAL_1(s8, befor_plus_plus, l);
		ANY_MATH_RET_CHECK_CAL_1(u8, befor_plus_plus, l);
		ANY_MATH_RET_CHECK_CAL_1(s16, befor_plus_plus, l);
		ANY_MATH_RET_CHECK_CAL_1(u16, befor_plus_plus, l);
		ANY_MATH_RET_CHECK_CAL_1(s32, befor_plus_plus, l);
		ANY_MATH_RET_CHECK_CAL_1(u32, befor_plus_plus, l);
		ANY_MATH_RET_CHECK_CAL_1(s64, befor_plus_plus, l);
		ANY_MATH_RET_CHECK_CAL_1(u64, befor_plus_plus, l);
		ANY_MATH_RET_CHECK_CAL_1(f32, befor_plus_plus, l);
		ANY_MATH_RET_CHECK_CAL_1(f64, befor_plus_plus, l);
		ANY_MATH_RET_CHECK_CAL_1(sint, befor_plus_plus, l);
		ANY_MATH_RET_CHECK_CAL_1(uint, befor_plus_plus, l);
		ANY_MATH_RET_CHECK_CAL_1(char, befor_plus_plus, l);


		return l;
	}

	template<typename L>
	static const L after_plus_plus(L& l)
	{
		assert(!is_any<L>());
		L rst(l);
		++l;
		return rst;
	}

	static const any_type after_plus_plus(any_type& l)
	{
		if(l.empty())
		{
			return l;
		}

		ANY_MATH_CONST_RET_CHECK_CAL_1(s8, after_plus_plus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(u8, after_plus_plus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(s16, after_plus_plus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(u16, after_plus_plus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(s32, after_plus_plus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(u32, after_plus_plus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(s64, after_plus_plus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(u64, after_plus_plus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(f32, after_plus_plus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(f64, after_plus_plus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(sint, after_plus_plus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(uint, after_plus_plus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(char, after_plus_plus, l);

		return l;
	}

	template<typename L>
	static L& befor_minus_minus(L& l)
	{
		assert(!this_type::is_any<L>());
		return --l;
	}

	static any_type& befor_minus_minus(any_type& l)
	{

		if(l.empty())
		{
			return l;
		}

		ANY_MATH_RET_CHECK_CAL_1(s8, befor_minus_minus, l);
		ANY_MATH_RET_CHECK_CAL_1(u8, befor_minus_minus, l);
		ANY_MATH_RET_CHECK_CAL_1(s16, befor_minus_minus, l);
		ANY_MATH_RET_CHECK_CAL_1(u16, befor_minus_minus, l);
		ANY_MATH_RET_CHECK_CAL_1(s32, befor_minus_minus, l);
		ANY_MATH_RET_CHECK_CAL_1(u32, befor_minus_minus, l);
		ANY_MATH_RET_CHECK_CAL_1(s64, befor_minus_minus, l);
		ANY_MATH_RET_CHECK_CAL_1(u64, befor_minus_minus, l);
		ANY_MATH_RET_CHECK_CAL_1(f32, befor_minus_minus, l);
		ANY_MATH_RET_CHECK_CAL_1(f64, befor_minus_minus, l);
		ANY_MATH_RET_CHECK_CAL_1(sint, befor_minus_minus, l);
		ANY_MATH_RET_CHECK_CAL_1(uint, befor_minus_minus, l);
		ANY_MATH_RET_CHECK_CAL_1(char, befor_minus_minus, l);

		return l;
	}


	template<typename L>
	static const L after_minus_minus(L& l)
	{
		assert(!is_any<L>());
		L rst = l;
		--l;
		return rst;
	}

	static const any_type after_minus_minus(any_type& l)
	{
		if(l.empty())
		{
			return l;
		}

		ANY_MATH_CONST_RET_CHECK_CAL_1(s8, after_minus_minus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(u8, after_minus_minus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(s16, after_minus_minus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(u16, after_minus_minus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(s32, after_minus_minus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(u32, after_minus_minus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(s64, after_minus_minus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(u64, after_minus_minus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(f32, after_minus_minus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(f64, after_minus_minus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(sint, after_minus_minus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(uint, after_minus_minus, l);
		ANY_MATH_CONST_RET_CHECK_CAL_1(char, after_minus_minus, l);

		return l;
	}

	template<typename L, typename R>
	static void set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		l = r;
	}

	static void set(any_type& l, const any_type& r)
	{
		if(l.empty()|| r.empty())
		{
			return;
		}
		
		ANY_MATH_NRET_CHECK_CAL_2_START(char, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s8, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u8, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s16, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u16, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, set, r)			
			ANY_MATH_NRET_CHECK_CAL_2(bool, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(f32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(f64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(sint, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(uint, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END


		ANY_MATH_NRET_CHECK_CAL_2_START(std::string, l)
			ANY_MATH_NRET_CHECK_CAL_2(std::string, set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(charset::utf8_string, l)
			ANY_MATH_NRET_CHECK_CAL_2(std::string, set, r)
			ANY_MATH_NRET_CHECK_CAL_2(charset::utf8_string, set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END
	}

	template<typename L, typename R>
	static const L plus(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		L rst = l + r;
		return rst;
	}

	static const any_type plus(const any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return l;
		}

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, plus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, plus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, plus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, plus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, plus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, plus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, plus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, plus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(f32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, plus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(f64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, plus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(sint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, plus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(uint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, plus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(std::string, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(std::string, plus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(charset::utf8_string, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(std::string, plus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(charset::utf8_string, plus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		return l;
	}

	template<typename L, typename R>
	static const void plus_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		l += r;
	}

	static const void plus_set(any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return;
		}

		ANY_MATH_NRET_CHECK_CAL_2_START(s8, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, plus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u8, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, plus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s16, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, plus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u16, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, plus_set, r)			
			ANY_MATH_NRET_CHECK_CAL_2(bool, plus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, plus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, plus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, plus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, plus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(f32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, plus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(f64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, plus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(sint, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, plus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(uint, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, plus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(char, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, plus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(std::string, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(std::string, plus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(charset::utf8_string, l)
			ANY_MATH_NRET_CHECK_CAL_2(std::string, plus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(charset::utf8_string, plus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END
	}

	template<typename L, typename R>
	static const L minus(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		L rst(l - r);
		return rst;
	}

	static const any_type minus(const any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return l;
		}

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, minus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, minus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, minus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, minus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, minus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, minus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, minus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, minus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(f32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, minus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(f64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, minus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(sint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, minus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(uint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, minus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, minus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		return l;
	}


	template<typename L, typename R>
	static void minus_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		l -= r;
	}

	static void minus_set(any_type& l, const any_type& r)
	{

		if(l.empty() || r.empty())
		{
			return;
		}

		ANY_MATH_NRET_CHECK_CAL_2_START(s8, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, minus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u8, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, minus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s16, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, minus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u16, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, minus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, minus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, minus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, minus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, minus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(f32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, minus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(f64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, minus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(sint, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, minus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(uint, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, minus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, minus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END
	}

	template<typename L, typename R>
	static const L multiplies(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		L rst(l * r);
		return rst;
	}

	static const any_type multiplies(const any_type& l, const any_type& r)
	{

		if(l.empty() || r.empty())
		{
			return l;
		}

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, multiplies, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, multiplies, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, multiplies, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, multiplies, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, multiplies, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, multiplies, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, multiplies, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, multiplies, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(f32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, multiplies, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(f64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, multiplies, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(sint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, multiplies, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(uint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, multiplies, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, multiplies, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		return l;
	}

	template<typename L, typename R>
	static void multiplies_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		l *= r;
	}

	static void multiplies_set(any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return;
		}

		ANY_MATH_NRET_CHECK_CAL_2_START(s8, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, multiplies_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u8, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, multiplies_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s16, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, multiplies_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u16, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, multiplies_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, multiplies_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, multiplies_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, multiplies_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, multiplies_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(f32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, multiplies_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(f64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, multiplies_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(sint, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, multiplies_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(uint, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, multiplies_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, multiplies_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END
	}

	template<typename L, typename R>
	static const L divides(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		if(!r)
		{
			return L();
		}
		
		L rst(l / r);
		return rst;
	}

	static const any_type divides(const any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return l;
		}

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, divides, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, divides, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, divides, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, divides, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, divides, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, divides, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, divides, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, divides, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(f32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, divides, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(f64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, divides, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(sint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, divides, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(uint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f32, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(f64, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, divides, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, divides, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		return l;
	}

	template<typename L, typename R>
	static void divides_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));

		if(!r)
		{
			return;
		}

		l /= r;
	}

	static void divides_set(any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return;
		}

		ANY_MATH_NRET_CHECK_CAL_2_START(s8, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, divides_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u8, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, divides_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s16, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, divides_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u16, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, divides_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, divides_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, divides_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, divides_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, divides_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(f32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, divides_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(f64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, divides_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(sint, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, divides_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(uint, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f32, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(f64, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, divides_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, divides_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END
	}

	template<typename L, typename R>
	static const L modulus(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));

		if(!r)
		{
			return L();
		}

		L rst = (l % r);
		return rst;
	}

	static const any_type modulus(const any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return l;
		}

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, modulus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, modulus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, modulus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, modulus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, modulus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, modulus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, modulus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, modulus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(sint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, modulus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(uint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, modulus, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, modulus, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END
		
		return l;
	}

	template<typename L, typename R>
	static void modulus_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));

		if(!r)
		{
			return;
		}

		l %= r;
	}

	static void modulus_set(any_type& l, const any_type& r)
	{

		if(l.empty() || r.empty())
		{
			return;
		}

		ANY_MATH_NRET_CHECK_CAL_2_START(s8, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, modulus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u8, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, modulus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s16, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, modulus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u16, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, modulus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, modulus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, modulus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, modulus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, modulus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(sint, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, modulus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(uint, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, modulus_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, modulus_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END
	}

	template<typename L, typename R>
	static bool logic_and(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		return l && r;
	}

	static bool logic_and(const any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return false;
		}

		bool bl = false, br = false;

		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(bool, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s8, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u8, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s16, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u16, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s32, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u32, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s64, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u64, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(f32, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(f64, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(sint, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(uint, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(char, bl, l)

		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(bool, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s8, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u8, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s16, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u16, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s32, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u32, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s64, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u64, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(f32, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(f64, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(sint, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(uint, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(char, br, r)

		return logic_and(bl, br);
	}

	template<typename L, typename R>
	static bool logic_or(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));

		return l || r;
	}

	static bool logic_or(const any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return false;
		}

		bool bl = false, br = false;

		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(bool, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s8, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u8, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s16, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u16, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s32, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u32, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s64, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u64, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(f32, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(f64, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(sint, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(uint, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(char, bl, l)

		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(bool, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s8, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u8, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s16, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u16, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s32, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u32, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s64, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u64, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(f32, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(f64, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(sint, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(uint, br, r)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(char, br, r)

		return logic_or(bl, br);
	}

	template<typename L>
	static bool logic_not(const L& l)
	{
		assert(!(this_type::is_any<L>()));
		return !l;
	}

	static bool logic_not(const any_type& l)
	{
		if(l.empty())
		{
			return false;
		}
		
		bool bl = false;

		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(bool, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s8, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u8, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s16, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u16, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s32, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u32, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(s64, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(u64, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(f32, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(f64, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(sint, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(uint, bl, l)
		ANY_LOGIC_RET_PARSE_CONST_CHECK_CAL(char, bl, l)

		return logic_not(bl);
	}

	template<typename L, typename R>
	static const L bit_and(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		L rst = l & r;
		return rst;
	}

	static const any_type bit_and(const any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return l;
		}

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(char, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_and, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_and, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_and, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_and, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_and, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_and, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_and, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_and, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_and, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(sint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_and, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(uint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_and, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(bool, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_and, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_and, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		return l;
	}

	template<typename L, typename R>
	static void bit_and_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		l &= r;
	}

	static void bit_and_set(any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return;
		}

		ANY_MATH_NRET_CHECK_CAL_2_START(char, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_and_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s8, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_and_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u8, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_and_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s16, l)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_and_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u16, l)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_and_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_and_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_and_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_and_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_and_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END


		ANY_MATH_NRET_CHECK_CAL_2_START(sint, l)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_and_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(uint, l)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_and_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(bool, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_and_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_and_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END
	}

	template<typename L, typename R>
	static const L bit_or(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		L rst = l | r;
		return rst;
	}

	static const any_type bit_or(const any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return l;
		}

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(char, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_or, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_or, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_or, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_or, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_or, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_or, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_or, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_or, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_or, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(sint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_or, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(uint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_or, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(bool, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_or, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_or, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		return l;
	}

	template<typename L, typename R>
	static void bit_or_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		l |= r;
	}

	static void bit_or_set(any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return;
		}

		ANY_MATH_NRET_CHECK_CAL_2_START(char, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_or_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s8, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_or_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u8, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_or_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s16, l)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_or_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u16, l)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_or_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_or_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_or_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_or_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_or_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END


		ANY_MATH_NRET_CHECK_CAL_2_START(sint, l)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_or_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(uint, l)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_or_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(bool, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_or_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_or_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END
	}

	template<typename L, typename R>
	static const L bit_xor(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		L rst = l ^ r;
		return rst;
	}

	static const any_type bit_xor(const any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return l;
		}

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(char, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_xor, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_xor, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_xor, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_xor, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_xor, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_xor, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_xor, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_xor, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_xor, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(sint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_xor, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(uint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_xor, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(bool, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_xor, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_xor, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		return l;
	}

	template<typename L, typename R>
	static void bit_xor_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		l ^= r;
	}

	static void bit_xor_set(any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return;
		}

		ANY_MATH_NRET_CHECK_CAL_2_START(char, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_xor_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s8, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_xor_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u8, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_xor_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s16, l)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_xor_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u16, l)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_xor_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_xor_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u32, l)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_xor_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_xor_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u64, l)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_xor_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END


		ANY_MATH_NRET_CHECK_CAL_2_START(sint, l)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_xor_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(uint, l)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_xor_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(bool, l)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_xor_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_xor_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

	}

	template<typename L, typename R>
	static const L bit_left_trans(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		L rst = l << r;
		return rst;
	}

	static any_type bit_left_trans(const any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return l;
		}

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(char, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_left_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_left_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_left_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_left_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_left_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_left_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_left_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_left_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_left_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(sint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_left_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(uint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_left_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_left_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		return l;
	}


	template<typename L, typename R>
	static void bit_left_trans_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		l <<= r;
	}

	static void bit_left_trans_set(any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return;
		}

		ANY_MATH_NRET_CHECK_CAL_2_START(char, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_left_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s8, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_left_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u8, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_left_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s16, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_left_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u16, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_left_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s32, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_left_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u32, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_left_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s64, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_left_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u64, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_left_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(sint, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_left_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(uint, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_left_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_left_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END
	}

	template<typename L, typename R>
	static const L bit_right_trans(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		L rst = l >> r;
		return rst;
	}

	static const any_type bit_right_trans(const any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return l;
		}

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(char, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_right_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_right_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u8, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_right_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_right_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u16, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_right_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_right_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u32, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_right_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(s64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_right_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(u64, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_right_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(sint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_right_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_START(uint, l)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(char, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u8, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u16, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u32, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(s64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(u64, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(sint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(uint, bit_right_trans, r)
			ANY_MATH_CONST_RET_CONST_CHECK_CAL_2(bool, bit_right_trans, r)
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_2_END

		return l;
	}

	template<typename L, typename R>
	static void bit_right_trans_set(L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		l >>= r;
	}

	static void bit_right_trans_set(any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return;
		}

		ANY_MATH_NRET_CHECK_CAL_2_START(char, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_right_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s8, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_right_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u8, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_right_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s16, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_right_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u16, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_right_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s32, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_right_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u32, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_right_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(s64, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_right_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(u64, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_right_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(sint, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_right_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END

		ANY_MATH_NRET_CHECK_CAL_2_START(uint, l)
			ANY_MATH_NRET_CHECK_CAL_2(char, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u8, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u16, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u32, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(s64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(u64, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(sint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(uint, bit_right_trans_set, r)
			ANY_MATH_NRET_CHECK_CAL_2(bool, bit_right_trans_set, r)
		ANY_MATH_NRET_CHECK_CAL_2_END
	}

	template<typename L>
	static L bit_not(const L& l)
	{
		assert(!(this_type::is_any<L>()));
		L rst = ~l;
		return rst;
	}

	static const any_type bit_not(const any_type& l)
	{
		if(l.empty())
		{
			return l;
		}

		ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(char, bit_not, l);
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(s8, bit_not, l);
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(u8, bit_not, l);
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(s16, bit_not, l);
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(u16, bit_not, l);
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(s32, bit_not, l);
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(u32, bit_not, l);
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(s64, bit_not, l);
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(u64, bit_not, l);
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(sint, bit_not, l);
		ANY_MATH_CONST_RET_CONST_CHECK_CAL_1(uint, bit_not, l);

		return l;
	}

	template<typename L, typename R>
	static bool equal_to(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		return l == r;
	}

	static bool equal_to(const any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return false;
		}

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(bool, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(char, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s8, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u8, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s16, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u16, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s32, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u32, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s64, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u64, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(f32, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(f64, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(sint, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(uint, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(std::string, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(std::string, equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(charset::utf8_string, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(std::string, equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(charset::utf8_string, equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		return false;
	}

	template<typename L, typename R>
	static bool not_equal_to(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		return l != r;
	}

	static bool not_equal_to(const any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return false;
		}

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(bool, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, not_equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(char, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, not_equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s8, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, not_equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u8, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, not_equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s16, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, not_equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u16, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, not_equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s32, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, not_equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u32, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, not_equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s64, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, not_equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u64, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, not_equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(f32, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, not_equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(f64, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, not_equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(sint, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, not_equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(uint, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, not_equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(std::string, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(std::string, not_equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(charset::utf8_string, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(std::string, not_equal_to, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(charset::utf8_string, not_equal_to, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		return false;
	}

	template<typename L, typename R>
	static bool less(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		return l < r;
	}
	
	static bool less(const any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return false;
		}

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(bool, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(char, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s8, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u8, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s16, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u16, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s32, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u32, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s64, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u64, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(f32, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(f64, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(sint, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(uint, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		return false;
	}

	template<typename L, typename R>
	static bool less_equal(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		return l <= r;
	}

	static bool less_equal(const any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return false;
		}

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(bool, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(char, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s8, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u8, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s16, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u16, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s32, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u32, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s64, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u64, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(f32, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(f64, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(sint, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(uint, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, less_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, less_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		return false;
	}

	template<typename L, typename R>
	static bool greater(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		return l > r;
	}

	static bool greater(const any_type& l, const any_type& r)
	{
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(bool, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(char, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s8, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u8, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s16, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u16, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s32, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u32, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s64, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u64, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(f32, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(f64, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(sint, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(uint, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		return false;
	}

	template<typename L, typename R>
	static bool greater_equal(const L& l, const R& r)
	{
		assert(!(this_type::is_have_any<L, R>()));
		return l >= r;
	}

	static bool greater_equal(const any_type& l, const any_type& r)
	{
		if(l.empty() || r.empty())
		{
			return false;
		}

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(bool, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(char, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s8, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u8, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s16, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u16, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s32, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u32, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(s64, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(u64, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(f32, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(f64, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(sint, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		ANY_LOGIC_RET_CONST_CHECK_CAL_2_START(uint, l)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(char, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u8, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u16, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(s64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(u64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f32, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(f64, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(sint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(uint, greater_equal, r)
			ANY_LOGIC_RET_CONST_CHECK_CAL_2(bool, greater_equal, r)
		ANY_LOGIC_RET_CONST_CHECK_CAL_2_END

		return false;
	}
	
protected:

	template<typename L>
	static bool is_any(void)
	{
		typedef L ltype;
		return typeid(ltype) == typeid(any_type);
	}

	template<typename L, typename R>
	static bool is_have_any(void) 
	{
		typedef L ltype;
		typedef R rtype;

		return typeid(ltype) == typeid(any_type) || typeid(rtype) == typeid(any_type);
	}

	template<typename L, typename R>
	static bool is_all_any(void)
	{
		typedef L ltype;
		typedef R rtype;

		return typeid(ltype) == typeid(any_type) && typeid(rtype) == typeid(any_type);
	}

};

} // namespace any_params
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

#endif //__YGGR_ANY_VAL_BASE_ANY_TYPE_OPERATOR_HPP__