//ops.hpp

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

#ifndef __YGGR_TYPE_AWARENESS_OPS_HPP__
#define __YGGR_TYPE_AWARENESS_OPS_HPP__

#include <yggr/type_awareness/opids_def.hpp>
#include <boost/mpl/bool.hpp>
#include <yggr/functional.hpp>
#include <yggr/ppex/typedef.hpp>

// op enable def
#define YGGR_AWARNS_SET_OP_ENABLE(__opid__, __type1__, __type2__) \
	namespace yggr { namespace type_awareness {
	template<> struct op_enable<__opid__, __type1__, __type2__> { \
		enum { value = __opid__ }; \
		typedef boost::mpl::type_ type; }; } }


namespace yggr
{
namespace type_awareness
{

template<u32 opid, typename T1, typename T2>
struct op_enable
{
	enum
	{
		value = opid;
	};
	typedef boost::mpl::false_ type;
};

template<typename T1, typename T2>
struct op_enable<0>
{
	enum
	{
		value = 0;
	};
	typedef boost::mpl::false_ type;
};

template<u32 opid, typename T>
struct opendable<opid, T, T>
{
	enum
	{
		value = opid;
	};
	typedef boost::mpl::true_ type;
};

template<typename T>
struct opendable<0, T, T>
{
	enum
	{
		value = 0;
	};
	typedef boost::mpl::false_ type;
};

} // namespace type_awareness
} // namespace yggr


// op type def
#define YGGR_AWARNS_OP_DEF(__opid__, __params_size__, __op__) \
	namespace yggr { namespace type_awareness { \
	template<> struct get_op< __opid__ > { \
		enum { id = __opid__, params_length = __params_size__ }; \
		template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__params_size__, typename T)> \
		struct op : public __op__<YGGR_PP_TEMPLATE_PARAMS_TYPES(__params_size__, T)> { }; }; } }

namespace yggr
{
namespace type_awareness
{

template<u32 opid>
struct get_op;

} // namespace type_awareness
} // namespace yggr

YGGR_AWARNS_OP_DEF(opids::E_PLUS, 2, plus)
YGGR_AWARNS_OP_DEF(opids::E_MINUS, 2, minus)
YGGR_AWARNS_OP_DEF(opids::E_MUL, 2, multiplies)
YGGR_AWARNS_OP_DEF(opids::E_DIV, 2, divides)
YGGR_AWARNS_OP_DEF(opids::E_MOD, 2, modulus)
YGGR_AWARNS_OP_DEF(opids::E_NEGATE, 1, negate)

YGGR_AWARNS_OP_DEF(opids::E_EQUAL_TO, 2, equal_to)
YGGR_AWARNS_OP_DEF(opids::E_NOT_EQUAL_TO, 2, not_equal_to)
YGGR_AWARNS_OP_DEF(opids::E_GREATER, 2, greater)
YGGR_AWARNS_OP_DEF(opids::E_LESS, 2, less)
YGGR_AWARNS_OP_DEF(opids::E_GREATER_EQUAL, 2, greater_equal)
YGGR_AWARNS_OP_DEF(opids::E_LESS_EQUAL, 2, less_equal)

YGGR_AWARNS_OP_DEF(opids::E_LOG_AND, 2, logical_and)
YGGR_AWARNS_OP_DEF(opids::E_LOG_OR, 2, logical_or)
YGGR_AWARNS_OP_DEF(opids::E_LOG_NOT, 1, logical_not)

#undef YGGR_AWARNS_OP_DEF


#endif //__YGGR_TYPE_AWARENESS_OPS_HPP__