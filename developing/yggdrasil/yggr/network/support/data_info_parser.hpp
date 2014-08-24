//data_info_parser.hpp

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

#ifndef __YGGR_NETWORK_SUPPORT_DATA_INFO_PARSER_HPP__
#define __YGGR_NETWORK_SUPPORT_DATA_INFO_PARSER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/task_center/task_data_id_def.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#define YGGR_PP_NETWORK_DATA_INFO_PARSER_DEF(__type__, __data_id__, __data_ver__) \
	namespace yggr { namespace network { namespace support { \
	template<typename Info_Def> \
	struct network_data_info_getter<Info_Def, __type__> { \
		typedef __type__ data_type; \
		typedef typename Info_Def::data_id_type data_id_type; \
		typedef typename Info_Def::data_ver_type data_ver_type; \
		typedef typename Info_Def::data_info_type data_info_type; \
		\
		inline static data_id_type data_id(void) { \
			return data_id_type(__data_id__); } \
		\
		inline static data_ver_type data_ver(void) { \
			return data_ver_type(__data_ver__); } \
		\
		inline static data_info_type data_info(void) { \
		return data_info_type(__data_id__, __data_ver__); } }; } } }


namespace yggr
{
namespace network
{
namespace support
{

template<typename Info_Def, typename T>
struct network_data_info_getter
{
	typedef T data_type;
	typedef typename Info_Def::data_id_type data_id_type;
	typedef typename Info_Def::data_ver_type data_ver_type;
	typedef typename Info_Def::data_info_type data_info_type;

	BOOST_MPL_ASSERT((boost::is_same<data_id_type, typename T::data_id_type>));
	BOOST_MPL_ASSERT((boost::is_same<data_ver_type, typename T::data_ver_type>));
	BOOST_MPL_ASSERT((boost::is_same<data_info_type, typename T::data_info_type>));

	inline static data_id_type data_id(void)
	{
		return data_type::s_data_id();
	}

	inline static data_ver_type data_ver(void)
	{
		return data_type::s_data_ver();
	}

	inline static data_info_type data_info(void)
	{
		return data_type::s_data_info();
	}
};

template<typename Info_Def>
struct network_data_info_parser
{
	typedef Info_Def info_def_type;
	typedef typename info_def_type::data_id_type data_id_type;
	typedef typename info_def_type::data_ver_type data_ver_type;
	typedef typename info_def_type::data_info_type data_info_type;

	template<typename T>
	inline static data_id_type data_id(void)
	{
		return network_data_info_getter<info_def_type, T>::data_id();
	}

	template<typename T>
	inline static data_ver_type data_ver(void)
	{
		return network_data_info_getter<info_def_type, T>::data_ver();
	}

	template<typename T>
	inline static data_info_type data_info(void)
	{
		return network_data_info_getter<info_def_type, T>::data_info();
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
} // namespace network
} // namespace yggr

YGGR_PP_NETWORK_DATA_INFO_PARSER_DEF(char, yggr::task_center::task_data_id_def::E_data_id_char, 1)
YGGR_PP_NETWORK_DATA_INFO_PARSER_DEF(s8, yggr::task_center::task_data_id_def::E_data_id_s8, 1)
YGGR_PP_NETWORK_DATA_INFO_PARSER_DEF(u8, yggr::task_center::task_data_id_def::E_data_id_u8, 1)
YGGR_PP_NETWORK_DATA_INFO_PARSER_DEF(wchar_t, yggr::task_center::task_data_id_def::E_data_id_wchar_t, 1)
YGGR_PP_NETWORK_DATA_INFO_PARSER_DEF(s16, yggr::task_center::task_data_id_def::E_data_id_s16, 1)
YGGR_PP_NETWORK_DATA_INFO_PARSER_DEF(u16, yggr::task_center::task_data_id_def::E_data_id_u16, 1)
YGGR_PP_NETWORK_DATA_INFO_PARSER_DEF(s32, yggr::task_center::task_data_id_def::E_data_id_s32, 1)
YGGR_PP_NETWORK_DATA_INFO_PARSER_DEF(u32, yggr::task_center::task_data_id_def::E_data_id_u32, 1)
YGGR_PP_NETWORK_DATA_INFO_PARSER_DEF(s64, yggr::task_center::task_data_id_def::E_data_id_s64, 1)
YGGR_PP_NETWORK_DATA_INFO_PARSER_DEF(u64, yggr::task_center::task_data_id_def::E_data_id_u64, 1)
YGGR_PP_NETWORK_DATA_INFO_PARSER_DEF(f32, yggr::task_center::task_data_id_def::E_data_id_f32, 1)
YGGR_PP_NETWORK_DATA_INFO_PARSER_DEF(f64, yggr::task_center::task_data_id_def::E_data_id_f64, 1)

#endif // __YGGR_NETWORK_SUPPORT_DATA_INFO_PARSER_HPP__
