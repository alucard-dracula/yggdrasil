//task_import.ipp

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

#ifndef __YGGR_TASK_CENTER_SUPPORT_TASK_IMPORT_HPP__
#define __YGGR_TASK_CENTER_SUPPORT_TASK_IMPORT_HPP__

#include <yggr/base/static_constant.hpp>

#include <yggr/task_center/task_info_def.hpp>
#include <yggr/task_center/support/task_data_info_parser.hpp>

#define _YGGR_TASK_CLASS_NAME( __type__ ) \
	inline static class_info_type class_info(void) { \
		return typeid( __type__ ); } \
	\
	inline static class_name_type class_name(void) { \
		return class_name_type(typeid( __type__ ).name()); }

//#define _YGGR_TASK_CLASS_NAME( __type__ ) \
//	inline static const class_info_type& class_info(void) { \
//		return typeid( __type__ ); } \
//	\
//	inline static class_name_type class_name(void) { \
//		return class_name_type(typeid( __type__ ).name()); }

#define _YGGR_TASK_IMPORT_ENUM( __data_id__, __data_ver__ ) \
	public: \
		YGGR_STATIC_CONSTANT( data_id_type, E_pak_id = __data_id__ ); \
		YGGR_STATIC_CONSTANT( data_ver_type, E_now_ver = __data_ver__ );

#define _YGGR_TASK_IMPORT_INNER_T( __base_type__ ) \
	public: \
		typedef ::yggr::task_center::support::task_data_info_parser< __base_type__ >::data_id_type data_id_type; \
		typedef ::yggr::task_center::support::task_data_info_parser< __base_type__ >::data_ver_type data_ver_type; \
		typedef ::yggr::task_center::support::task_data_info_parser< __base_type__ >::data_info_type data_info_type; \
		typedef ::yggr::task_center::support::task_data_info_parser< __base_type__ >::class_info_type class_info_type; \
		typedef ::yggr::task_center::support::task_data_info_parser< __base_type__ >::class_name_type class_name_type;

#define _YGGR_TASK_IMPORT_INNER_TPL( __base_type__ ) \
	public: \
		typedef typename ::yggr::task_center::support::task_data_info_parser< __base_type__ >::data_id_type data_id_type; \
		typedef typename ::yggr::task_center::support::task_data_info_parser< __base_type__ >::data_ver_type data_ver_type; \
		typedef typename ::yggr::task_center::support::task_data_info_parser< __base_type__ >::data_info_type data_info_type; \
		typedef typename ::yggr::task_center::support::task_data_info_parser< __base_type__ >::class_info_type class_info_type; \
		typedef typename ::yggr::task_center::support::task_data_info_parser< __base_type__ >::class_name_type class_name_type;

#define _YGGR_TASK_IMPORT_MFOO( __this_type__, __data_id__, __data_ver__ ) \
	public: \
		inline static data_id_type data_id(void) { \
			return data_id_type( __data_id__ ); } \
		\
		inline static data_ver_type data_ver(void) { \
			return data_ver_type( __data_ver__ ); } \
		\
		inline static data_info_type data_info(void) { \
			return data_info_type(data_id_type( __data_id__ ), \
									data_ver_type( __data_ver__ ) ); } \
		\
		inline static data_info_type data_info(const __this_type__&) { \
			return __this_type__::data_info(); } \
		\
		_YGGR_TASK_CLASS_NAME( __this_type__ )

# define _YGGR_GET_CAL_TYPE( __type__ ) \
	public: \
		inline static ::yggr::u32 cal_type(void) { return __type__; } 


#define YGGR_CAL_TASK_IMPORT( __base_type__, __this_type__, __data_id__, __data_ver__, __cal_type__ ) \
	_YGGR_TASK_IMPORT_INNER_T( __base_type__ ) \
	_YGGR_TASK_IMPORT_ENUM( __data_id__, __data_ver__ ) \
	_YGGR_TASK_IMPORT_MFOO( __this_type__, (__this_type__::E_pak_id), (__this_type__::E_now_ver) ) \
	_YGGR_GET_CAL_TYPE( __cal_type__ )

#define YGGR_CAL_TASK_IMPORT_TPL( __base_type__, __this_type__, __data_id__, __data_ver__, __cal_type__ ) \
	_YGGR_TASK_IMPORT_INNER_TPL( __base_type__ ) \
	_YGGR_TASK_IMPORT_ENUM( __data_id__, __data_ver__ ) \
	_YGGR_TASK_IMPORT_MFOO( __this_type__, (__this_type__::E_pak_id), (__this_type__::E_now_ver) ) \
	_YGGR_GET_CAL_TYPE( __cal_type__ )

#define YGGR_FAST_CAL_TASK_IMPORT( __base_type__, __this_type__, __data_id__, __data_ver__ ) \
	YGGR_CAL_TASK_IMPORT(__base_type__, __this_type__, __data_id__, __data_ver__, ::yggr::task_center::task_cal_type::E_CAL_FAST)

#define YGGR_FAST_CAL_TASK_IMPORT_TPL( __base_type__, __this_type__, __data_id__, __data_ver__ ) \
	YGGR_CAL_TASK_IMPORT_TPL(__base_type__, __this_type__, __data_id__, __data_ver__, ::yggr::task_center::task_cal_type::E_CAL_FAST)

#define YGGR_SLOW_CAL_TASK_IMPORT( __base_type__, __this_type__, __data_id__, __data_ver__ ) \
	YGGR_CAL_TASK_IMPORT(__base_type__, __this_type__, __data_id__, __data_ver__, ::yggr::task_center::task_cal_type::E_CAL_SLOW)

#define YGGR_SLOW_CAL_TASK_IMPORT_TPL( __base_type__, __this_type__, __data_id__, __data_ver__ ) \
	YGGR_CAL_TASK_IMPORT_TPL(__base_type__, __this_type__, __data_id__, __data_ver__, ::yggr::task_center::task_cal_type::E_CAL_SLOW)

#define YGGR_INHERIT_CAL_TASK_IMPORT(__this_type__) \
	public: \
		_YGGR_TASK_CLASS_NAME( __this_type__ )

#endif //__YGGR_TASK_CENTER_SUPPORT_TASK_IMPORT_HPP__
