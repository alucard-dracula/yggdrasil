//exception_sort.hpp

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

#ifndef __YGGR_EXCEPTION_EXCEPTION_SORT_HPP__
#define __YGGR_EXCEPTION_EXCEPTION_SORT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

namespace yggr
{
namespace exception
{

class exception_sort
{
public:
	YGGR_STATIC_CONSTANT(u32, E_code_start_1 = 0x60000000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_1 = 0x60001000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_2 = 0x60001000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_2 = 0x60002000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_3 = 0x60002000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_3 = 0x60003000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_4 = 0x60003000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_4 = 0x60004000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_5 = 0x60004000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_5 = 0x60005000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_6 = 0x60005000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_6 = 0x60006000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_7 = 0x60006000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_7 = 0x60007000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_8 = 0x60007000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_8 = 0x60008000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_9 = 0x60008000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_9 = 0x60009000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_10 = 0x60009000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_10 = 0x6000a000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_11 = 0x6000a000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_11 = 0x6000b000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_12 = 0x6000b000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_12 = 0x6000c000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_13 = 0x6000d000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_13 = 0x6000e000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_14 = 0x6000e000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_14 = 0x6000f000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_15 = 0x6000f000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_15 = 0x60010000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_16 = 0x60010000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_16 = 0x60020000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_17 = 0x60020000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_17 = 0x60030000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_18 = 0x60030000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_18 = 0x60040000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_19 = 0x60040000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_19 = 0x60050000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_20 = 0x60050000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_20 = 0x60060000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_21 = 0x60060000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_21 = 0x60070000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_22 = 0x60070000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_22 = 0x60080000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_23 = 0x60080000); // used here
	YGGR_STATIC_CONSTANT(u32, E_code_end_23 = 0x60090000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_24 = 0x60090000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_24 = 0x600a0000);

	YGGR_STATIC_CONSTANT(u32, E_code_start_25 = 0x600a0000);
	YGGR_STATIC_CONSTANT(u32, E_code_end_25 = 0x600b0000);

	YGGR_STATIC_CONSTANT(u32, E_code_stl_error = 0x7000000);
	YGGR_STATIC_CONSTANT(u32, E_code_dtl_db_error = 0x80000000);
	YGGR_STATIC_CONSTANT(u32, E_code_dtl_root_error = 0x90000000);
	YGGR_STATIC_CONSTANT(u32, E_code_luabind_error = 0xa0000000);
	YGGR_STATIC_CONSTANT(u32, E_code_runtime_error = 0xb0000000);
	YGGR_STATIC_CONSTANT(u32, E_code_seh_error = 0xc0000000);

	YGGR_STATIC_CONSTANT(u32, E_code_user_error = 0xe0000000);
};

} //namespace exception
} //namespace yggr

#endif //__YGGR_EXCEPTION_EXCEPTION_SORT_HPP__

