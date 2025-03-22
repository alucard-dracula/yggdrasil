//array_indexes_permutation_parity.hpp

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

#ifndef __YGGR_MATH_ARRAY_INDEXES_PERMUTATION_PARITY_HPP__
#define __YGGR_MATH_ARRAY_INDEXES_PERMUTATION_PARITY_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

namespace yggr
{
namespace math
{

// def
template<s_size_type I_IDX, s_size_type IDX_SIZE, bool ASC_OR_DESC = true>
struct sign_array_indexes;

template<s_size_type I_IDX, s_size_type IDX_SIZE, bool ASC_OR_DESC = true>
struct array_indexes_permutation_parity;

namespace detail
{

template<s_size_type I_IDX, s_size_type J_IDX, s_size_type IDX_SIZE, s_size_type E_IDX, s_size_type STEP, bool RUNABLE>
struct indexes_loop;

} // namespace detail

//impl
template<s_size_type I_IDX, s_size_type IDX_SIZE, bool ASC_OR_DESC>
struct sign_array_indexes
{

	YGGR_STATIC_CONSTANT(
		s_size_type, 
		value = (0 == array_indexes_permutation_parity<I_IDX, IDX_SIZE, ASC_OR_DESC>::value % 2)? 1 : -1);
};

template<s_size_type I_IDX, s_size_type IDX_SIZE, bool ASC_OR_DESC>
struct array_indexes_permutation_parity
{
private:
	YGGR_STATIC_CONSTANT(s_size_type, step = (ASC_OR_DESC? 1 : -1));
	YGGR_STATIC_CONSTANT(s_size_type, now = I_IDX);
	YGGR_STATIC_CONSTANT(s_size_type, cmp = (step < 0 && now == 0? IDX_SIZE - 1 : (now + step) % IDX_SIZE));
	YGGR_STATIC_CONSTANT(
		s_size_type, 
		end = (step < 0? 
				now < (IDX_SIZE - 1)? 
					IDX_SIZE + now + step * (IDX_SIZE - 1)
					: now + step * (IDX_SIZE - 1)
				: (now + step * (IDX_SIZE - 1)) % IDX_SIZE));
public:
	YGGR_STATIC_CONSTANT(
		s_size_type, 
		value = (detail::indexes_loop< now, cmp, IDX_SIZE, end, step, (I_IDX < IDX_SIZE) >::value));
};

template<s_size_type I_IDX, bool ASC_OR_DESC>
struct array_indexes_permutation_parity<I_IDX, 0, ASC_OR_DESC>
{
	YGGR_STATIC_CONSTANT(s_size_type, value = 0);
};

template<s_size_type I_IDX, bool ASC_OR_DESC>
struct array_indexes_permutation_parity<I_IDX, 1, ASC_OR_DESC>
{
	YGGR_STATIC_CONSTANT(s_size_type, value = 0);
};

template<s_size_type I_IDX, bool ASC_OR_DESC>
struct array_indexes_permutation_parity<I_IDX, 2, ASC_OR_DESC>
{
private:
	YGGR_STATIC_CONSTANT(s_size_type, step = (ASC_OR_DESC? 1 : -1));
	YGGR_STATIC_CONSTANT(s_size_type, now = I_IDX);
	YGGR_STATIC_CONSTANT(s_size_type, cmp = (step < 0 && now == 0? 2 - 1 : (now + step) % 2));

public:
	YGGR_STATIC_CONSTANT(s_size_type, value = (now > cmp? 1 : 0));
};

namespace detail
{

template<s_size_type I_IDX, s_size_type J_IDX, s_size_type IDX_SIZE, s_size_type E_IDX, s_size_type STEP, bool RUNABLE>
struct indexes_loop
{
private:
	YGGR_STATIC_CONSTANT(s_size_type, now = I_IDX);
	YGGR_STATIC_CONSTANT(s_size_type, cmp = J_IDX);
	YGGR_STATIC_CONSTANT(s_size_type, cmp_next = ((STEP < 0 && cmp == 0)? (IDX_SIZE - 1) : ((cmp + STEP) % IDX_SIZE)));

public:
	YGGR_STATIC_CONSTANT(
		s_size_type, 
		value = ((now > cmp? 1 : 0) + (indexes_loop<now, cmp_next, IDX_SIZE, E_IDX, STEP, (I_IDX < IDX_SIZE) >::value)));
};

template<s_size_type I_IDX, s_size_type J_IDX, s_size_type IDX_SIZE, s_size_type E_IDX, s_size_type STEP>
struct indexes_loop<I_IDX, J_IDX, IDX_SIZE, E_IDX, STEP, false>
{
	YGGR_STATIC_CONSTANT(s_size_type, value = 0);
};

template<s_size_type I_IDX, s_size_type IDX_SIZE, s_size_type E_IDX, s_size_type STEP>
struct indexes_loop<I_IDX, E_IDX, IDX_SIZE, E_IDX, STEP, true>
{
private:
	YGGR_STATIC_CONSTANT(s_size_type, now = I_IDX);
	YGGR_STATIC_CONSTANT(s_size_type, cmp = E_IDX);
	YGGR_STATIC_CONSTANT(s_size_type, now_next = (STEP < 0 && now == 0? IDX_SIZE - 1 : (now + STEP) % IDX_SIZE));
	YGGR_STATIC_CONSTANT(s_size_type, cmp_next = (STEP < 0 && now_next == 0? IDX_SIZE - 1 : (now_next + STEP) % IDX_SIZE));

public:
	YGGR_STATIC_CONSTANT(
		s_size_type, 
		value = ((now > cmp? 1 : 0) + (detail::indexes_loop<now_next, cmp_next, IDX_SIZE, E_IDX, STEP, (I_IDX < IDX_SIZE) >::value)));
};

template<s_size_type J_IDX, s_size_type IDX_SIZE, s_size_type E_IDX, s_size_type STEP>
struct indexes_loop<E_IDX, J_IDX, IDX_SIZE, E_IDX, STEP, true>
{
	YGGR_STATIC_CONSTANT(s_size_type, value = 0);
};

} // namespace detail

} // namespace math
} // namespace yggr


#endif // __YGGR_MATH_ARRAY_INDEXES_PERMUTATION_PARITY_HPP__
