//upper_types.hpp

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

#ifndef __YGGR_BASE_UPPER_TYPES_HPP__
#define __YGGR_BASE_UPPER_TYPES_HPP__

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4624)
#endif //_MSC_VER

#include <yggr/base/yggrdef.h>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/make_signed.hpp>
#include <boost/type_traits/make_unsigned.hpp>

#define YGGR_PP_UPPER_FLOAT_DEF(__src__, __dst__) \
	namespace yggr { \
	template<> struct upper_float< __src__ > { \
		typedef __dst__ value_type; }; } \
    \
    namespace yggr { \
	template<> struct upper_float< const __src__ > { \
		typedef const __dst__ value_type; }; }

namespace yggr
{

template<typename T>
struct upper_float;
//{
//    BOOST_MPL_ASSERT((boost::mpl::false_));
//	typedef T value_type;
//};

} // namespace yggr

YGGR_PP_UPPER_FLOAT_DEF(s8, f32)
YGGR_PP_UPPER_FLOAT_DEF(u8, f32)
YGGR_PP_UPPER_FLOAT_DEF(s16, f32)
YGGR_PP_UPPER_FLOAT_DEF(u16, f32)
YGGR_PP_UPPER_FLOAT_DEF(s32, f32)
YGGR_PP_UPPER_FLOAT_DEF(u32, f32)

YGGR_PP_UPPER_FLOAT_DEF(f32, f32)

YGGR_PP_UPPER_FLOAT_DEF(s64, f64)
YGGR_PP_UPPER_FLOAT_DEF(u64, f64)

YGGR_PP_UPPER_FLOAT_DEF(f64, f64)

//-----------------upper_float end-----------------------

//-----------------upper_signed end-----------------------

#define YGGR_PP_UPPER_SIGNED_DEF(__src__, __dst__) \
	namespace yggr { \
	template<> struct upper_signed< __src__ > { \
		typedef __dst__ value_type; }; }

namespace yggr
{
template<typename T>
struct upper_signed
{
	typedef typename boost::make_signed<T>::type value_type;
};

} // namespace yggr

YGGR_PP_UPPER_SIGNED_DEF(f32, f32)
YGGR_PP_UPPER_SIGNED_DEF(f64, f64)

//-----------------upper_signed end-----------------------

//-----------------upper_unsigned end-----------------------

#define YGGR_PP_UPPER_UNSIGNED_DEF(__src__, __dst__) \
	namespace yggr { \
	template<> struct upper_unsigned< __src__ > { \
		typedef __dst__ value_type; }; }

namespace yggr
{
template<typename T>
struct upper_unsigned
{
	typedef typename boost::make_unsigned<T>::type value_type;
};

} // namespace yggr


//-----------------upper_unsigned end-----------------------

#define YGGR_PP_UPPER_INTEGER_DEF(__src__, __dst__) \
	namespace yggr { \
	template<> struct upper_integer< __src__ > { \
		typedef __dst__ value_type; }; }

namespace yggr
{

template<typename T>
struct upper_integer
{
	typedef typename boost::make_unsigned<T>::type value_type;
};

} // namespace yggr

YGGR_PP_UPPER_INTEGER_DEF(f32, s32)
YGGR_PP_UPPER_INTEGER_DEF(f64, s64)

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

#endif //__YGGR_BASE_UPPER_TYPES_HPP__
