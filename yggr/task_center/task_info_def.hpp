//task_info_def.hpp

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

#ifndef __YGGR_TASK_CENTER_TASK_INFO_DEF_HPP__
#define __YGGR_TASK_CENTER_TASK_INFO_DEF_HPP__

#include <yggr/base/static_constant.hpp>
#include <yggr/packet/packet_info.hpp>
#include <yggr/charset/string.hpp>

#include <yggr/task_center/support/task_data_info_parser.hpp>

#include <typeinfo>

namespace yggr
{
namespace task_center
{

struct task_cal_type
{
	YGGR_STATIC_CONSTANT(u32, E_CAL_FAST = 1);
	YGGR_STATIC_CONSTANT(u32, E_CAL_SLOW = 2);
};

struct runtime_task_type
{
	YGGR_STATIC_CONSTANT(u32, E_CAL_CONDITION = 1);
	YGGR_STATIC_CONSTANT(u32, E_CAL_RESULT = 2);
};

template<typename ID, typename Ver, typename ClassName = yggr::string>
struct task_data_info
{
public:
	typedef ID data_id_type;
	typedef Ver data_ver_type;
	typedef yggr::packet::packet_info<data_id_type, data_ver_type> data_info_type;
	typedef const std::type_info& class_info_type;
	typedef ClassName class_name_type;
};

typedef task_data_info<u32, u16, yggr::string> default_task_data_info_type;

} // namespace task_center
} // namespace yggr

#endif // __YGGR_TASK_CENTER_TASK_INFO_DEF_HPP__