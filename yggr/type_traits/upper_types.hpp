//upper_types.hpp

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

#ifndef __YGGR_TYPE_TRAITS_UPPER_TYPES_HPP__
#define __YGGR_TYPE_TRAITS_UPPER_TYPES_HPP__

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4624)
#endif //_MSC_VER

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/bytes_to_types.hpp>

#include <boost/type_traits/make_signed.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/mpl/if.hpp>

#define YGGR_PP_UPPER_FLOAT_DEF(__src__, __dst__) \
	namespace yggr { \
	template<> struct upper_float< __src__ > { \
		typedef __dst__ type; }; }

namespace yggr
{

namespace detail
{
template<typename T>
struct upper_float_detail
	: public
		boost::mpl::if_
		<
			boost::mpl::bool_<(sizeof(T) < sizeof(f32))>,
			typename bytes_to_float<sizeof(f32)>::type,
			typename bytes_to_float<sizeof(T)>::type
		>
{
};

} // namespace detail

template<typename T>
struct upper_float
	: public detail::upper_float_detail<T>
{
};

} // namespace yggr


//-----------------upper_float end-----------------------

//-----------------upper_signed end-----------------------

#define YGGR_PP_UPPER_SIGNED_DEF(__src__, __dst__) \
	namespace yggr { \
	template<> struct upper_signed< __src__ > { \
		typedef __dst__ type; }; }

namespace yggr
{
namespace detail
{

template<typename T, bool IsFloat>
struct upper_signed_switch;

template<typename T>
struct upper_signed_switch<T, false>
	: public boost::make_signed<T>
{
};

template<typename T>
struct upper_signed_switch<T, true>
	: public bytes_to_float<sizeof(T)>
{
};

template<typename T>
struct upper_signed_detail
	: public upper_signed_switch<T, boost::is_floating_point<T>::value>
{
};

} // namespace detail

template<typename T>
struct upper_signed
	: public detail::upper_signed_detail<T>
{
};

} // namespace yggr

//-----------------upper_signed end-----------------------

//-----------------upper_unsigned end-----------------------

#define YGGR_PP_UPPER_UNSIGNED_DEF(__src__, __dst__) \
	namespace yggr { \
	template<> struct upper_unsigned< __src__ > { \
		typedef __dst__ type; }; }

namespace yggr
{

namespace detail
{

template<typename T>
struct upper_unsigned_detail
	: public boost::make_unsigned<T>
{
};

} // namespace detail

template<typename T>
struct upper_unsigned
	: public detail::upper_unsigned_detail<T>
{
};

} // namespace yggr


//-----------------upper_unsigned end-----------------------

#define YGGR_PP_UPPER_INTEGER_DEF(__src__, __dst__) \
	namespace yggr { \
	template<> struct upper_integer< __src__ > { \
		typedef __dst__ type; }; }

namespace yggr
{

namespace detail
{

template<typename T>
struct upper_integer_detail
	: public 
		boost::mpl::if_
		<
			boost::is_floating_point<T>,
			typename bytes_to_signed<sizeof(T)>::type,
			T
		>
{
};

} // namespace detail

template<typename T>
struct upper_integer
	: public detail::upper_integer_detail<T>
{
};

} // namespace yggr


#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

#endif //__YGGR_TYPE_TRAITS_UPPER_TYPES_HPP__
