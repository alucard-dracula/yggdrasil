//task_data_info_parser.hpp

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

#ifndef __YGGR_TASK_CENTER_SUPPORT_TASK_DATA_INFO_PARSER_HPP__
#define __YGGR_TASK_CENTER_SUPPORT_TASK_DATA_INFO_PARSER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/task_center/task_data_id_def.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <boost/type_traits/is_same.hpp>

//#define YGGR_PP_TASK_DATA_INFO_PARSER_DEF(__type__, __data_id__, __data_ver__) \
//	namespace yggr { namespace task_center { namespace support { \
//	template<typename Info_Def> \
//	struct task_data_info_getter<Info_Def, __type__> { \
//		typedef __type__ data_type; \
//		typedef typename Info_Def::data_id_type data_id_type; \
//		typedef typename Info_Def::data_ver_type data_ver_type; \
//		typedef typename Info_Def::data_info_type data_info_type; \
//		typedef typename Info_Def::class_info_type class_info_type; \
//		typedef typename Info_Def::class_name_type class_name_type; \
//		\
//		inline static data_id_type data_id(void) { \
//			return data_id_type(__data_id__); } \
//		\
//		inline static data_ver_type data_ver(void) { \
//			return data_ver_type(__data_ver__); } \
//		\
//		inline static data_info_type data_info(void) { \
//			return data_info_type(__data_id__, __data_ver__); } \
//		\
//		inline static const class_info_type& class_info(void) { \
//			return typeid(__type__); } \
//		\
//		inline static class_name_type class_name(void) { \
//			return class_name_type(typeid(__type__).name()); } }; } } }

#define YGGR_PP_TASK_DATA_INFO_PARSER_DEF(__type__, __data_id__, __data_ver__) \
	namespace yggr { namespace task_center { namespace support { \
	template<typename Info_Def> \
	struct task_data_info_getter<Info_Def, __type__> { \
		typedef __type__ data_type; \
		typedef typename Info_Def::data_id_type data_id_type; \
		typedef typename Info_Def::data_ver_type data_ver_type; \
		typedef typename Info_Def::data_info_type data_info_type; \
		typedef typename Info_Def::class_info_type class_info_type; \
		typedef typename Info_Def::class_name_type class_name_type; \
		\
		inline static data_id_type data_id(void) { \
			return data_id_type(__data_id__); } \
		\
		inline static data_ver_type data_ver(void) { \
			return data_ver_type(__data_ver__); } \
		\
		inline static data_info_type data_info(void) { \
			return data_info_type(__data_id__, __data_ver__); } \
		\
		inline static class_info_type class_info(void) { \
			return typeid(__type__); } \
		\
		inline static class_name_type class_name(void) { \
			return class_name_type(typeid(__type__).name()); } }; } } }

namespace yggr
{
namespace task_center
{
namespace support
{

template<typename Info_Def, typename T>
struct task_data_info_getter
{
	typedef T data_type;
	typedef typename Info_Def::data_id_type data_id_type;
	typedef typename Info_Def::data_ver_type data_ver_type;
	typedef typename Info_Def::data_info_type data_info_type;
	typedef typename Info_Def::class_info_type class_info_type;
	typedef typename Info_Def::class_name_type class_name_type;

	BOOST_MPL_ASSERT((boost::is_same<data_id_type, typename T::data_id_type>));
	BOOST_MPL_ASSERT((boost::is_same<data_ver_type, typename T::data_ver_type>));
	BOOST_MPL_ASSERT((boost::is_same<data_info_type, typename T::data_info_type>));
	BOOST_MPL_ASSERT((boost::is_same<class_info_type, typename T::class_info_type>));
	BOOST_MPL_ASSERT((boost::is_same<class_name_type, typename T::class_name_type>));

	inline static data_id_type data_id(void)
	{
		return data_type::data_id();
	}

	inline static data_ver_type data_ver(void)
	{
		return data_type::data_ver();
	}

	inline static data_info_type data_info(void)
	{
		return data_type::data_info();
	}

	//inline static const class_info_type& class_info(void)
	//{
	//	return data_type::class_info();
	//}

	inline static class_info_type class_info(void)
	{
		return data_type::class_info();
	}

	inline static class_name_type class_name(void)
	{
		return data_type::class_name();
	}
};

template<typename Info_Def>
struct task_data_info_parser
{
	typedef Info_Def info_def_type;
	typedef typename info_def_type::data_id_type data_id_type;
	typedef typename info_def_type::data_ver_type data_ver_type;
	typedef typename info_def_type::data_info_type data_info_type;
	typedef typename info_def_type::class_info_type class_info_type;
	typedef typename info_def_type::class_name_type class_name_type;

	template<typename T> inline
	static data_id_type data_id(void)
	{
		return task_data_info_getter<info_def_type, T>::data_id();
	}

	template<typename T> inline
	static data_ver_type data_ver(void)
	{
		return task_data_info_getter<info_def_type, T>::data_ver();
	}

	template<typename T> inline
	static data_info_type data_info(void)
	{
		return task_data_info_getter<info_def_type, T>::data_info();
	}

	//template<typename T> inline
	//static const class_info_type& class_info(void)
	//{
	//	return task_data_info_getter<info_def_type, T>::class_info();
	//}

	template<typename T> inline
	static class_info_type class_info(void)
	{
		return task_data_info_getter<info_def_type, T>::class_info();
	}

	template<typename T> inline
	static class_name_type class_name(void)
	{
		return task_data_info_getter<info_def_type, T>::class_name();
	}

	inline static const data_id_type& data_id(const data_info_type& info)
	{
		return info.template get<0>();
	}

	inline static const data_ver_type& data_ver(const data_info_type& info)
	{
		return info.template get<1>();
	}
};

} // namespace support
} // namespace task_center
} // namespace yggr

YGGR_PP_TASK_DATA_INFO_PARSER_DEF(char, task_data_id_def::E_data_id_char, 1)
YGGR_PP_TASK_DATA_INFO_PARSER_DEF(s8, task_data_id_def::E_data_id_s8, 1)
YGGR_PP_TASK_DATA_INFO_PARSER_DEF(u8, task_data_id_def::E_data_id_u8, 1)

#ifndef YGGR_NO_CWCHAR
YGGR_PP_TASK_DATA_INFO_PARSER_DEF(wchar_t, task_data_id_def::E_data_id_wchar_t, 1)
#endif // YGGR_NO_CWCHAR

YGGR_PP_TASK_DATA_INFO_PARSER_DEF(s16, task_data_id_def::E_data_id_s16, 1)
YGGR_PP_TASK_DATA_INFO_PARSER_DEF(u16, task_data_id_def::E_data_id_u16, 1)
YGGR_PP_TASK_DATA_INFO_PARSER_DEF(s32, task_data_id_def::E_data_id_s32, 1)
YGGR_PP_TASK_DATA_INFO_PARSER_DEF(u32, task_data_id_def::E_data_id_u32, 1)
YGGR_PP_TASK_DATA_INFO_PARSER_DEF(s64, task_data_id_def::E_data_id_s64, 1)
YGGR_PP_TASK_DATA_INFO_PARSER_DEF(u64, task_data_id_def::E_data_id_u64, 1)
YGGR_PP_TASK_DATA_INFO_PARSER_DEF(f32, task_data_id_def::E_data_id_f32, 1)
YGGR_PP_TASK_DATA_INFO_PARSER_DEF(f64, task_data_id_def::E_data_id_f64, 1)

#ifndef YGGR_NO_CHAR8_T
YGGR_PP_TASK_DATA_INFO_PARSER_DEF(c8, task_data_id_def::E_data_id_c8, 1)
#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_CHAR16_T
YGGR_PP_TASK_DATA_INFO_PARSER_DEF(c16, task_data_id_def::E_data_id_c16, 1)
#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T
YGGR_PP_TASK_DATA_INFO_PARSER_DEF(c32, task_data_id_def::E_data_id_c32, 1)
#endif // YGGR_NO_CHAR32_T

#endif // __YGGR_TASK_CENTER_SUPPORT_TASK_DATA_INFO_PARSER_HPP__
