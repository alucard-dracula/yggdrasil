//task_info.hpp

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

#ifndef __YGGR_TASK_CENTER_TASK_INFO_HPP__
#define __YGGR_TASK_CENTER_TASK_INFO_HPP__

#include <yggr/any_val/any.hpp>

#include <yggr/task_center/task_info_def.hpp>
#include <yggr/task_center/support/task_data_info_parser.hpp>


namespace yggr
{
namespace task_center
{

template<typename TaskDataInfo,
			template<typename _Task_Data_Info>
			class Parser = yggr::task_center::support::task_data_info_parser>
struct task_data_send_typeid_getter
{
	typedef TaskDataInfo task_data_info_type;
	typedef Parser<task_data_info_type> parser_type;
	typedef typename parser_type::data_info_type result_type;

	template<typename T> inline
	result_type operator()(void) const
	{
		return parser_type::template data_info<T>();
	}
};

template<typename TaskDataInfo,
			template<typename _Task_Data_Info>
			class Parser = yggr::task_center::support::task_data_info_parser>
struct task_data_recv_typeid_getter
{
	typedef TaskDataInfo task_data_info_type;
	typedef Parser<task_data_info_type> parser_type;
	typedef typename parser_type::class_name_type result_type;

	template<typename T> inline
	result_type operator()(void) const
	{
		return parser_type::template class_name<T>();
	}
};

} // namespace task_center
} // namespace yggr


#define YGGR_PP_TASK_CENTER_BACK_TASK(__handler__, __runner__, __err__, __run_type__, __uids__, __val__) \
	{ ::yggr::any tmp_any(__val__); \
		(__handler__)((__runner__), (__err__), static_cast<yggr::u32>((__run_type__)), \
						boost::cref((__uids__)), (__val__.data_info()), \
						(__val__.cal_type()), (boost::ref(tmp_any))); }



#endif // __YGGR_TASK_CENTER_TASK_INFO_HPP__
