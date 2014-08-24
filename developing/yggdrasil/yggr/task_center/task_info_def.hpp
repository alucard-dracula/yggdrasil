//task_info_def.hpp

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

#ifndef __YGGR_TASK_CENTER_TASK_INFO_DEF_HPP__
#define __YGGR_TASK_CENTER_TASK_INFO_DEF_HPP__

#include <typeinfo>
#include <yggr/base/yggrdef.h>
#include <yggr/packet/packet_info.hpp>
#include <yggr/charset/string.hpp>

namespace yggr
{
namespace task_center
{

struct task_cal_type
{
	enum
	{
		//E_RESULT = 0,
		E_CAL_FAST = 1,
		E_CAL_SLOW,

		E_compile_u32 = 0xffffffff
	};
};

struct runtime_task_type
{
	enum
	{
		E_CAL_CONDITION = 1,
		E_CAL_RESULT,

		E_compile_u32 = 0xffffffff
	};
};

template<typename ID, typename Ver, typename Class_Name = std::string>
class task_data_info
{
public:
	typedef ID data_id_type;
	typedef Ver data_ver_type;
	typedef yggr::packet::packet_info<data_id_type, data_ver_type> data_info_type;
	typedef const std::type_info& class_info_type;
	typedef Class_Name class_name_type;
};

//template<u32 cal_t, typename Task_Data_Info>
//class task_info
//{
//public:
//	typedef Task_Data_Info task_data_info_type;
//	typedef typename task_data_info_type::data_id_type data_id_type;
//	typedef typename task_data_info_type::data_ver_type data_ver_type;
//	typedef typename task_data_info_type::data_info_type data_info_type;
//	typedef typename task_data_info_type::class_info_type class_info_type;
//	typedef typename task_data_info_type::class_name_type class_name_type;
//	
//
//private:
//	typedef task_info this_type;
//
//public:
//	task_info(void) {}
//	~task_info(void) {}
//
//	static u32 s_cal_type(void)
//	{
//		return cal_t;
//	}
//
//	u32 cal_type(void) const
//	{
//		return cal_t;
//	}

	/*
	need definition this foos
	static s_data_id
	static s_data_ver
	static s_data_info
	static s_class_type
	*/
//};


typedef task_data_info<u32, u16, std::string> default_task_data_info_type;
//typedef task_info<(u32)task_cal_type::E_CAL_FAST, default_task_data_info_type> default_cdt_fast_task_info_type;
//typedef task_info<(u32)task_cal_type::E_CAL_SLOW, default_task_data_info_type> default_cdt_slow_task_info_type;

} // namespace task_center
} // namespace yggr

#endif // __YGGR_TASK_CENTER_TASK_INFO_DEF_HPP__