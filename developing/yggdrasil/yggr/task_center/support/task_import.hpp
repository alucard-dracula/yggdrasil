//task_import.ipp

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

#ifndef __YGGR_TASK_CENTER_SUPPORT_TASK_IMPORT_HPP__
#define __YGGR_TASK_CENTER_SUPPORT_TASK_IMPORT_HPP__

/*
\
	static u32 s_cal_type(void) { return cal_t; } \
	\
	u32 cal_type(void) const { return cal_t; } \
	\
*/
#include <yggr/task_center/task_info_def.hpp>

#define _YGGR_TASK_IMPORT(__base_type__, __this_type__, __data_id__, __data_ver__) \
	public: \
		typedef __base_type__::data_id_type data_id_type; \
		typedef __base_type__::data_ver_type data_ver_type; \
		typedef __base_type__::data_info_type data_info_type; \
		typedef __base_type__::class_info_type class_info_type; \
		typedef __base_type__::class_name_type class_name_type; \
		\
		inline data_id_type data_id(void) const { \
			return data_id_type(__data_id__); } \
		\
		inline data_ver_type data_ver(void) const { \
			return data_ver_type(__data_ver__); } \
		\
		inline data_info_type data_info(void) const { \
			return data_info_type(data_id_type(__data_id__), \
									data_ver_type(__data_ver__) ); } \
		\
		inline class_info_type class_info(void) const { \
			return typeid(__this_type__); } \
		\
		inline class_name_type class_name(void) const { \
			return class_name_type(typeid(__this_type__).name()); } \
		\
		inline static data_id_type s_data_id(void) { \
			return data_id_type(__data_id__); } \
		\
		inline static data_ver_type s_data_ver(void) { \
			return data_ver_type(__data_ver__); } \
		\
		inline static data_info_type s_data_info(void) { \
			return data_info_type(data_id_type(__data_id__), \
									data_ver_type(__data_ver__) ); } \
		\
		inline static class_info_type s_class_info(void) { \
			return typeid(__this_type__); } \
		\
		inline static class_name_type s_class_name(void) { \
			return class_name_type(typeid(__this_type__).name()); }

# define _YGGR_GET_CAL_TYPE(__type__) \
	public: \
		inline static ::yggr::u32 s_cal_type(void) { return __type__; } \
		\
		::yggr::u32 cal_type(void) const { return __type__; }

#define YGGR_FAST_CAL_TASK_IMPORT(__base_type__, __this_type__, __data_id__, __data_ver__) \
	_YGGR_TASK_IMPORT(__base_type__, __this_type__, __data_id__, __data_ver__) \
	_YGGR_GET_CAL_TYPE(yggr::task_center::task_cal_type::E_CAL_FAST)

#define YGGR_SLOW_CAL_TASK_IMPORT(__base_type__, __this_type__, __data_id__, __data_ver__) \
	_YGGR_TASK_IMPORT(__base_type__, __this_type__, __data_id__, __data_ver__) \
	_YGGR_GET_CAL_TYPE(yggr::task_center::task_cal_type::E_CAL_SLOW)

#endif //__YGGR_TASK_CENTER_SUPPORT_TASK_IMPORT_HPP__
