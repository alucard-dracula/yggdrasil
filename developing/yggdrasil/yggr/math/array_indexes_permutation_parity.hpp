//array_indexes_permutation_parity.hpp

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

#ifndef __YGGR_MATH_INDEXES_PERMUTATION_PARITY_HPP__
#define __YGGR_MATH_INDEXES_PERMUTATION_PARITY_HPP__

#include <yggr/base/yggrdef.h>

namespace yggr
{
namespace math
{

//template<s_size_type X>
//struct abs_t
//{
//	static const s_size_type value = X < 0? -X : X;
//};

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
	enum
	{
		value = (0 == array_indexes_permutation_parity<I_IDX, IDX_SIZE, ASC_OR_DESC>::value % 2)? 1 : -1,
		E_compile_u32 = 0xffffffff
	};
};

template<s_size_type I_IDX, s_size_type IDX_SIZE, bool ASC_OR_DESC>
struct array_indexes_permutation_parity
{
private:
	enum
	{
		step = (ASC_OR_DESC? 1 : -1),
		now = I_IDX,
		cmp = (step < 0 && now == 0? IDX_SIZE - 1 : (now + step) % IDX_SIZE),
		end = (step < 0
					? now < (IDX_SIZE - 1)
						? IDX_SIZE + now + step * (IDX_SIZE - 1)
							: now + step * (IDX_SIZE - 1)
					: (now + step * (IDX_SIZE - 1)) % IDX_SIZE),

		E_compile_params_u32 = 0xffffffff
	};
public:
	enum
	{
		//value = 0,
		value = (detail::indexes_loop< now, cmp, IDX_SIZE, end, step, (I_IDX < IDX_SIZE) >::value),
		E_compile_value_u32 = 0xffffffff
	};
};

template<s_size_type I_IDX, bool ASC_OR_DESC>
struct array_indexes_permutation_parity<I_IDX, 0, ASC_OR_DESC>
{
	enum
	{
		value = 0,
		E_compile_u32 = 0xffffffff
	};
};

template<s_size_type I_IDX, bool ASC_OR_DESC>
struct array_indexes_permutation_parity<I_IDX, 1, ASC_OR_DESC>
{
	enum
	{
		value = 0,
		E_compile_u32 = 0xffffffff
	};
};

template<s_size_type I_IDX, bool ASC_OR_DESC>
struct array_indexes_permutation_parity<I_IDX, 2, ASC_OR_DESC>
{
private:
	enum
	{
		step = (ASC_OR_DESC? 1 : -1),
		now = I_IDX,
		cmp = (step < 0 && now == 0? 2 - 1 : (now + step) % 2),

		E_compile_params_u32 = 0xffffffff
	};
public:
	enum
	{
		value = (now > cmp? 1 : 0),
		E_compile_value_u32 = 0xffffffff
	};
};

namespace detail
{

template<s_size_type I_IDX, s_size_type J_IDX, s_size_type IDX_SIZE, s_size_type E_IDX, s_size_type STEP, bool RUNABLE>
struct indexes_loop
{
private:
	enum
	{
		now = I_IDX,
		cmp = J_IDX,
		next_cmp = ((STEP < 0 && cmp == 0)? (IDX_SIZE - 1) : ((cmp + STEP) % IDX_SIZE)),

		E_compile_params_u32 = 0xffffffff
	};
public:
	enum
	{
		//value = 0,
		value = ((now > cmp? 1 : 0) + (indexes_loop<now, next_cmp, IDX_SIZE, E_IDX, STEP, (I_IDX < IDX_SIZE) >::value)),
		E_compile_value_u32 = 0xffffffff
	};
};

template<s_size_type I_IDX, s_size_type J_IDX, s_size_type IDX_SIZE, s_size_type E_IDX, s_size_type STEP>
struct indexes_loop<I_IDX, J_IDX, IDX_SIZE, E_IDX, STEP, false>
{
	enum
	{
		value = 0,
		E_compile_u32 = 0xffffffff
	};
};

template<s_size_type I_IDX, s_size_type IDX_SIZE, s_size_type E_IDX, s_size_type STEP>
struct indexes_loop<I_IDX, E_IDX, IDX_SIZE, E_IDX, STEP, true>
{
private:
	enum
	{
		now = I_IDX,
		cmp = E_IDX,

		now_next = (STEP < 0 && now == 0? IDX_SIZE - 1 : (now + STEP) % IDX_SIZE),
		cmp_next = (STEP < 0 && now_next == 0? IDX_SIZE - 1 : (now_next + STEP) % IDX_SIZE),

		E_compile_params_u32 = 0xffffffff
	};
public:
	enum
	{
		//value = 0,
		value = ((now > cmp? 1 : 0) + (detail::indexes_loop<now_next, cmp_next, IDX_SIZE, E_IDX, STEP, (I_IDX < IDX_SIZE) >::value)),
		E_compile_value_u32 = 0xffffffff
	};
};

template<s_size_type J_IDX, s_size_type IDX_SIZE, s_size_type E_IDX, s_size_type STEP>
struct indexes_loop<E_IDX, J_IDX, IDX_SIZE, E_IDX, STEP, true>
{
	enum
	{
		value = 0,
		E_compile_value_u32 = 0xffffffff
	};
};

} // namespace detail

} // namespace math
} // namespace yggr


#endif // __YGGR_MATH_INDEXES_PERMUTATION_PARITY_HPP__
