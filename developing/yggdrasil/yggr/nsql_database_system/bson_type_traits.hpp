//bson_type_traits.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_TYPE_TRAITS_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_TYPE_TRAITS_HPP__

#include <yggr/nsql_database_system/mongodb_config.h>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <yggr/ppex/typedef.hpp>

namespace yggr
{
namespace nsql_database_system
{

template<typename T>
struct bson_value_typeid
{
	enum
	{
		value = bson_value_type::E_BSON_TYPE_EOO
	};
};

template<typename T>
struct get_overlap_bson_value_typeid
{
	enum
	{
		value = ( bson_value_typeid<T>::value
							== bson_value_type::E_BSON_TYPE_SPLICE_OBJECT
					? bson_value_type::E_BSON_TYPE_OBJECT
					: bson_value_typeid<T>::value)
	};
};

template<typename T>
struct is_bson_inside_type
{
public:
	typedef boost::mpl::bool_<
				bson_value_typeid< typename boost::remove_const<T>::type >::value
					!= bson_value_type::E_BSON_TYPE_EOO > type;
	enum
	{
		value = bson_value_typeid< typename boost::remove_const<T>::type >::value
					!= bson_value_type::E_BSON_TYPE_EOO
	};
};

template<typename T>
struct is_bson_inside_splice
{
	typedef boost::mpl::bool_<
				(bson_value_typeid< typename boost::remove_const<T>::type >::value
									== bson_value_type::E_BSON_TYPE_ARRAY
				||
				bson_value_typeid< typename boost::remove_const<T>::type >::value
									== bson_value_type::E_BSON_TYPE_SPLICE_OBJECT)
								> type;
	enum
	{
		value = (bson_value_typeid<typename boost::remove_const<T>::type>::value
					== bson_value_type::E_BSON_TYPE_ARRAY
				||
				bson_value_typeid< typename boost::remove_const<T>::type >::value
									== bson_value_type::E_BSON_TYPE_SPLICE_OBJECT)
	};
};

struct bson_mark_splice {};
struct bson_mark_nil {};

template<typename T>
struct bson_value_mark
{
	typedef
		typename boost::mpl::if_<typename is_bson_inside_splice<T>::type,
										bson_mark_splice, bson_mark_nil >::type type;
};

} // namespace nsql_database_system
} // namespace yggr


#define YGGR_PP_BSON_VALUE_TYPE_ID(__type__, __id__) \
namespace yggr { namespace nsql_database_system { \
template<> struct bson_value_typeid< __type__ > { enum { value = __id__ }; }; \
template<> struct bson_value_typeid< const __type__ > { enum { value = __id__ }; }; }}

#define YGGR_PP_CONTAINER_BSON_VALUE_TYPE_ID( __template_n__, __type__, __id__ ) \
namespace yggr { namespace nsql_database_system { \
template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__template_n__, typename T)> \
struct bson_value_typeid< __type__ < YGGR_PP_TEMPLATE_PARAMS_TYPES(__template_n__, T) > > { \
	enum { value = __id__ }; };}}

YGGR_PP_BSON_VALUE_TYPE_ID(bool, bson_value_type::E_BSON_TYPE_BOOL)

YGGR_PP_BSON_VALUE_TYPE_ID(s8, bson_value_type::E_BSON_TYPE_INT)
YGGR_PP_BSON_VALUE_TYPE_ID(char, bson_value_type::E_BSON_TYPE_INT)

#if !((defined _MSC_VER) && (_MSC_VER < 1300))
	YGGR_PP_BSON_VALUE_TYPE_ID(wchar_t, bson_value_type::E_BSON_TYPE_INT)
#endif // (defined _MSC_VER) && (_MSC_VER < 1300)

YGGR_PP_BSON_VALUE_TYPE_ID(u8, bson_value_type::E_BSON_TYPE_INT)
YGGR_PP_BSON_VALUE_TYPE_ID(s16, bson_value_type::E_BSON_TYPE_INT)
YGGR_PP_BSON_VALUE_TYPE_ID(u16, bson_value_type::E_BSON_TYPE_INT)
YGGR_PP_BSON_VALUE_TYPE_ID(s32, bson_value_type::E_BSON_TYPE_INT)
YGGR_PP_BSON_VALUE_TYPE_ID(u32, bson_value_type::E_BSON_TYPE_INT)

//YGGR_PP_BSON_VALUE_TYPE_ID(sint, bson_value_type::E_BSON_TYPE_INT)
//YGGR_PP_BSON_VALUE_TYPE_ID(uint, bson_value_type::E_BSON_TYPE_INT)

YGGR_PP_BSON_VALUE_TYPE_ID(s64, bson_value_type::E_BSON_TYPE_LONG)
YGGR_PP_BSON_VALUE_TYPE_ID(u64, bson_value_type::E_BSON_TYPE_LONG)

YGGR_PP_BSON_VALUE_TYPE_ID(f32, bson_value_type::E_BSON_TYPE_DOUBLE)
YGGR_PP_BSON_VALUE_TYPE_ID(f64, bson_value_type::E_BSON_TYPE_DOUBLE)

YGGR_PP_BSON_VALUE_TYPE_ID(std::string, bson_value_type::E_BSON_TYPE_STRING)


#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_TYPE_TRAITS_HPP__
