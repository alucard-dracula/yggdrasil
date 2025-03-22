//number.hpp

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

#ifndef __YGGR_TYPE_TRAITS_NUMBER_T_HPP__
#define __YGGR_TYPE_TRAITS_NUMBER_T_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/type_traits/is_enum.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/min_max.hpp>
#include <boost/mpl/size_t.hpp>

#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>

namespace yggr
{

// integer_t
template<std::size_t>
struct tag_integer_t
{
	typedef s64 type;
};

//uinteger_t
template<std::size_t>
struct tag_uinteger_t
{
	typedef u64 type;
};

// real_t
template<std::size_t>
struct tag_real_t
{
	typedef f64 type;
};

} // namespace yggr

#define YGGR_PP_INTEGER_T_DEF(__size__, __type__) \
	namespace yggr { \
	template<> \
	struct tag_integer_t< __size__ > { \
		typedef __type__ type; }; }

#define YGGR_PP_UINTEGER_T_DEF(__size__, __type__) \
	namespace yggr { \
	template<> \
	struct tag_uinteger_t< __size__ > { \
		typedef __type__ type; }; }

#define YGGR_PP_REAL_T_DEF(__size__, __type__) \
	namespace yggr { \
	template<> \
	struct tag_real_t< __size__ > { \
		typedef __type__ type; }; }


// integer

YGGR_PP_INTEGER_T_DEF(1, s8)
YGGR_PP_INTEGER_T_DEF(2, s16)
YGGR_PP_INTEGER_T_DEF(4, s32)
YGGR_PP_INTEGER_T_DEF(8, s64)

// uinteger

YGGR_PP_UINTEGER_T_DEF(1, u8)
YGGR_PP_UINTEGER_T_DEF(2, u16)
YGGR_PP_UINTEGER_T_DEF(4, u32)
YGGR_PP_UINTEGER_T_DEF(8, u64)

// real
YGGR_PP_REAL_T_DEF(4, f32)
YGGR_PP_REAL_T_DEF(8, f64)

#undef YGGR_PP_INTEGER_T_DEF
#undef YGGR_PP_UINTEGER_T_DEF
#undef YGGR_PP_REAL_T_DEF

#define YGGR_PP_INTEGER_T( __size__ ) \
	yggr::tag_integer_t< static_cast<std::size_t>(__size__) >

#define YGGR_PP_UINTEGER_T( __size__ ) \
	yggr::tag_uinteger_t< static_cast<std::size_t>(__size__) >

#define YGGR_PP_REAL_T( __size__ ) \
	yggr::tag_real_t< static_cast<std::size_t>(__size__) >

#define YGGR_PP_INTEGER_T_TYPE( __size__ ) \
	YGGR_PP_INTEGER_T( __size__ )::type

#define YGGR_PP_UINTEGER_T_TYPE( __size__ ) \
	YGGR_PP_UINTEGER_T( __size__ )::type

#define YGGR_PP_REAL_T_TYPE( __size__ ) \
	YGGR_PP_REAL_T( __size__ )::type

#define YGGR_PP_INTEGER_T_TYPE_TPL( __size__ ) \
	typename YGGR_PP_INTEGER_T_TYPE( __size__ )

#define YGGR_PP_UINTEGER_T_TYPE_TPL( __size__ ) \
	typename YGGR_PP_UINTEGER_T_TYPE( __size__ )

#define YGGR_PP_REAL_T_TYPE_TPL( __size__ ) \
	typename YGGR_PP_REAL_T_TYPE( __size__ )

namespace yggr
{

template<typename T>
struct tag_number_t
	: public
		boost::mpl::if_
		<
			boost::mpl::or_
			<
				boost::is_signed<T>,
				::yggr::is_enum<T>
			>,
			tag_integer_t<static_cast<std::size_t>(sizeof(T))>,
			typename 
				boost::mpl::if_
				<
					boost::is_unsigned<T>,
					tag_uinteger_t<static_cast<std::size_t>(sizeof(T))>,
					tag_real_t<static_cast<std::size_t>(sizeof(T))>
				>::type
		>::type
{
};

template<std::size_t Low, typename T>
struct tag_low_number_t
	: public
		boost::mpl::if_
		<
			boost::mpl::or_
			<
				boost::is_signed<T>,
				::yggr::is_enum<T>
			>,
			tag_integer_t
			<
				static_cast<std::size_t>
				(
					boost::mpl::max
					<
						boost::mpl::size_t<Low>,
						boost::mpl::size_t<static_cast<std::size_t>(sizeof(T))>
					>::type::value
				)
			>,
			typename 
				boost::mpl::if_
				<
					boost::is_unsigned<T>,
					tag_uinteger_t
					<
						static_cast<std::size_t>
						(
							boost::mpl::max
							<
								boost::mpl::size_t<Low>,
								boost::mpl::size_t<static_cast<std::size_t>(sizeof(T))>
							>::type::value
						)
					>,
					tag_real_t
					<
						static_cast<std::size_t>
						(
							boost::mpl::max
							<
								boost::mpl::size_t<Low>,
								boost::mpl::size_t<static_cast<std::size_t>(sizeof(T))>
							>::type::value
						)
					>
				>::type
		>::type
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(0 == (Low % 2))>));
	YGGR_STATIC_CONSTANT(std::size_t, value = Low);
};

template<std::size_t Upper, typename T>
struct tag_upper_number_t
	: public
		boost::mpl::if_
		<
			boost::mpl::or_
			<
				boost::is_signed<T>,
				::yggr::is_enum<T>
			>,
			tag_integer_t
			<
				static_cast<std::size_t>
				(
					boost::mpl::min
					<
						boost::mpl::size_t<Upper>,
						boost::mpl::size_t<static_cast<std::size_t>(sizeof(T))>
					>::type::value
				)
			>,
			typename 
				boost::mpl::if_
				<
					boost::is_unsigned<T>,
					tag_uinteger_t
					<
						static_cast<std::size_t>
						(
							boost::mpl::min
							<
								boost::mpl::size_t<Upper>,
								boost::mpl::size_t<static_cast<std::size_t>(sizeof(T))>
							>::type::value
						)
					>,
					tag_real_t
					<
						static_cast<std::size_t>
						(
							boost::mpl::min
							<
								boost::mpl::size_t<Upper>,
								boost::mpl::size_t<static_cast<std::size_t>(sizeof(T))>
							>::type::value
						)
					>
				>::type
		>::type
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(0 == (Upper % 2))>));
	YGGR_STATIC_CONSTANT(std::size_t, value = Upper);
};

} // namespace yggr

#define YGGR_PP_NUMBER_T_DEF(__type__, __number_t__) \
	namespace yggr { \
	template<> \
	struct tag_integer_t< __type__ > { \
		typedef __number_t__ type; }; }

#define YGGR_PP_NUMBER_T( __type__ ) \
	yggr::tag_number_t< __type__ >

#define YGGR_PP_NUMBER_T_TYPE( __type__ ) \
	YGGR_PP_NUMBER_T( __type__ )::type

#define YGGR_PP_NUMBER_T_TYPE_TPL( __type__ ) \
	typename YGGR_PP_NUMBER_T_TYPE( __type__ )

#define YGGR_PP_LOW_NUMBER_T( __size__, __type__ ) \
	yggr::tag_low_number_t< __size__, __type__ >

#define YGGR_PP_LOW_NUMBER_T_TYPE( __size__, __type__ ) \
	YGGR_PP_LOW_NUMBER_T( __size__, __type__ )::type

#define YGGR_PP_LOW_NUMBER_T_TYPE_TPL( __size__, __type__ ) \
	typename YGGR_PP_LOW_NUMBER_T_TYPE( __size__, __type__ )

#define YGGR_PP_UPPER_NUMBER_T( __size__, __type__ ) \
	yggr::tag_upper_number_t< __size__, __type__ >

#define YGGR_PP_UPPER_NUMBER_T_TYPE( __size__, __type__ ) \
	YGGR_PP_UPPER_NUMBER_T( __size__, __type__ )::type

#define YGGR_PP_UPPER_NUMBER_T_TYPE_TPL( __size__, __type__ ) \
	typename YGGR_PP_UPPER_NUMBER_T_TYPE( __size__, __type__ )


#endif //__YGGR_TYPE_TRAITS_NUMBER_T_HPP__

