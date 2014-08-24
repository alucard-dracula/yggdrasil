//task_data_id_def.hpp

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

#ifndef __YGGR_TASK_CENTER_TASK_DATA_ID_DEF_HPP__
#define __YGGR_TASK_CENTER_TASK_DATA_ID_DEF_HPP__

namespace yggr
{
namespace task_center
{

struct task_data_id_def
{
	enum
	{
		E_data_id_start = 0x00008000,
		E_data_id_char,
		E_data_id_s8,
		E_data_id_u8,
		E_data_id_wchar_t,
		E_data_id_s16,
		E_data_id_u16,
		E_data_id_s32,
		E_data_id_u32,
		E_data_id_s64,
		E_data_id_u64,
		E_data_id_f32,
		E_data_id_f64,

		E_data_id_user = E_data_id_f64 + 100,
		E_compile_u32 = 0xffffffff
	};
};

} // namespace task_center
} // namespace yggr

#endif // __YGGR_TASK_CENTER_TASK_DATA_ID_DEF_HPP__